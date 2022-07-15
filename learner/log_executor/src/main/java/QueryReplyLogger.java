import java.io.*;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.lang.*;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import org.apache.commons.logging.Log;
import org.apache.commons.codec.binary.Hex;

class QueryReplyLogger {
  private List<Testcases> testcases = null;
  private List<List<QueryReplyPair>> pairs = null;
  private String outputRootDir;
  private String outputDir;
  private String ueModel;
  private Log logger;

  QueryReplyLogger(String outputDir, Log logger) {
    File directory = new File(outputDir);
    if (!directory.exists()) {
      directory.mkdir();
    }

    setLogger(logger);
    this.testcases = new ArrayList<>();
    this.pairs = new ArrayList<>();
    this.outputRootDir = outputDir;
    this.ueModel = null;
  }

  public void storeLog() throws IOException {
    Iterator i = this.testcases.iterator();
    Iterator j = this.pairs.iterator();
    Iterator k;
    int num = 0;

    Testcases tcs;
    List<QueryReplyPair> plst;
    QueryReplyPair tmp = null;

    while (i.hasNext()) {
      num++;
      String filename = this.outputDir + "/result." + num;
      BufferedWriter writer = new BufferedWriter(new FileWriter(filename));

      tcs = (Testcases) i.next();
      if (j.hasNext())
        plst = (List<QueryReplyPair>) j.next();
      else {
        logger.error("Should not happen");
        break;
      }

      logger.info("Testcase #" + num);
      logger.info("Testcase: " + tcs.getOriginalTestcase());

      writer.write("Testcase: " + tcs.getOriginalTestcase() + "\n");
      writer.write("Message:\n");

      k = plst.iterator();
      if (k.hasNext()) {
        tmp = (QueryReplyPair) k.next();
        logger.info("Query: " + tmp.getQueryName() + " / Reply: " + tmp.getReplyName());
        writer.write("  Query: " + tmp.getQueryName() + "\n");
        writer.write("  Reply: " + tmp.getReplyName() + "\n");
      }

      writer.close();
    }
  }

  public void setUEModel(String ueModel) {
    File directory;
    this.ueModel = ueModel;
    this.outputDir = this.outputRootDir + "/" + ueModel;

    directory = new File(this.outputDir);
    if (!directory.exists()) {
      directory.mkdir();
    }
  }

  public void setLogger(Log logger) {
    this.logger = logger;
  }

  public void addLog(Testcases ts, List<QueryReplyPair> pair) {
    this.testcases.add(ts);
    this.pairs.add(pair);
  }

  public List<Testcases> getTestcases() {
    return this.testcases;
  }

  public Iterator getLogIterator() {
    return pairs.iterator();
  }
}

class QueryReplyPair {
  private Testcase testcase;
  private MessageLog query;
  private MessageLog reply;
  static Log logger;

  QueryReplyPair(Testcase testcase, MessageLog query, MessageLog reply, Log logger) {
    setLogger(logger);
    this.testcase = testcase;
    this.query = query;
    this.reply = reply;
  }

  QueryReplyPair(Testcase testcase, MessageLog query, MessageLog reply) {
    this(testcase, query, reply, null);
  }

  public MessageLog getQuery() {
    return this.query;
  }

  public MessageLog getReply() {
    return this.reply;
  }

  public String getQueryName() {
    return this.query.getName();
  }

  public String getReplyName() {
    return this.reply.getName();
  }

  public String getIspi() {
    return this.reply.getIspi();
  }

  public String getRspi() {
    return this.reply.getIspi();
  }

  public void setLogger(Log logger) {
    this.logger = logger;
  }
}

class MessageLog {
  private Testcase testcase;
  private MessageLogType type;
  private MessageLog parent;
  private List<MessageLog> sub = null;
  private Iterator iter = null;
  private String ispi = null;
  private String rspi = null;
  private String name = null;
  private ValueType vtype;
  private String value = null;
  static Log logger;

  MessageLog(Testcase testcase, MessageLogType type, MessageLog parent, Log logger) {
    setLogger(logger);
    this.testcase = testcase;
    this.type = type;
    this.parent = parent;
    this.vtype = ValueType.NONE;
  }

  MessageLog(Testcase testcase, MessageLogType type, Log logger) {
    this(testcase, type, null, logger);
  }

  MessageLog(Testcase testcase, MessageLogType type, MessageLog parent) {
    this(testcase, type, parent, null);
  }

  MessageLog(Testcase testcase, MessageLogType type) {
    this(testcase, type, null, null);
  }

  public MessageLog addSubmessage(MessageLogType type) {
    MessageLog ret;
    if (this.type == MessageLogType.ATTRIBUTE) {
      ret = new MessageLog(this.testcase, type, this.getParent(), this.logger);
      this.getParent().sub.add(ret);
    } else {
      ret = new MessageLog(this.testcase, type, this, this.logger);
      if (this.sub == null) {
        this.sub = new ArrayList<MessageLog>();
      }
      this.sub.add(ret);
    }
    return ret;
  }

  public Iterator getIterator() {
    if (this.sub != null) {
      this.iter = this.sub.iterator();
    }
    return this.iter;
  }

  void setLogger(Log logger)
  {
    this.logger = logger;
  }

  MessageLogType getType() {
    return this.type;
  }

  void setType(MessageLogType type) {
    this.type = type;
  }

  boolean hasParent() {
    return this.parent != null;
  }

  MessageLog getParent() {
    return this.parent;
  }

  String getIspi() {
    MessageLog tmp = this;
    while (true) {
      if (tmp.getType() == MessageLogType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    return tmp.ispi;
  }

  void setIspi(String ispi) {
    MessageLog tmp = this;
    while (true) {
      if (tmp.getType() == MessageLogType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    tmp.ispi = ispi;
  }

  String getRspi() {
    MessageLog tmp = this;
    while (true) {
      if (tmp.getType() == MessageLogType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    return tmp.rspi;
  }

  void setRspi(String rspi) {
    MessageLog tmp = this;
    while (true) {
      if (tmp.getType() == MessageLogType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    tmp.rspi = rspi;
  }

  String getName() {
    return this.name;
  }

  void setName(String name) {
    this.name = name;
  }

  ValueType getValueType() {
    return this.vtype;
  }

  void setValueType(String vtype) {
    int type = Integer.parseInt(vtype);

    switch (type) {
      case 1:
        this.vtype = ValueType.NONE;
        break;
      case 2:
        this.vtype = ValueType.INTEGER;
        break;
      case 3:
        this.vtype = ValueType.UINT16;
        break;
      case 4:
        this.vtype = ValueType.STRING;
        break;
      default:
        this.vtype = ValueType.NONE;
    }
  }

  String getValue() {
    return this.value;
  }

  void setValue(String value) {
    this.value = value;
  }
}

enum MessageLogType {
  MESSAGE("message") {
    public void printMessageLogType(Log logger) {
      logger.info("This is a reply of a message type");
    }
  },
  PAYLOAD("payload") {
    public void printMessageLogType(Log logger) {
      logger.info("This is a reply of a payload type");
    }
  },
  ATTRIBUTE("attribute") {
    public void printMessageLogType(Log logger) {
      logger.info("This is a reply of a attribute type");
    }
  };

  private final String type;

  MessageLogType(String type) {
    this.type = type;
  }

  @Override public String toString() {
    return type;
  }

  public abstract void printMessageLogType(Log logger);
}
