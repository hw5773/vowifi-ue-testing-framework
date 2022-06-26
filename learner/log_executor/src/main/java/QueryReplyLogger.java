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

}

class Reply {
  private Query query;
  private ReplyType type;
  private Reply parent;
  private List<Reply> sub = null;
  private Iterator iter = null;
  private String ispi = null;
  private String rspi = null;
  private String name = null;
  private ValueType vtype;
  private String value = null;
  static Log logger;

  Reply(Query query, ReplyType type, Reply parent, Log logger) {
    setLogger(logger);
    this.query = query;
    this.type = type;
    this.parent = parent;
    this.vtype = ValueType.NONE;
  }

  Reply(Query query, ReplyType type, Log logger) {
    this(query, type, null, logger);
  }

  Reply(Query query, ReplyType type, Reply parent) {
    this(query, type, parent, null);
  }

  Reply(Query query, ReplyType type) {
    this(query, type, null, null);
  }

  public Reply addSubmessage(ReplyType type) {
    Reply ret;
    if (this.type == ReplyType.ATTRIBUTE) {
      ret = new Reply(this.query, type, this.getParent(), this.logger);
      this.getParent().sub.add(ret);
    } else {
      ret = new Reply(this.query, type, this, this.logger);
      if (this.sub == null) {
        this.sub = new ArrayList<Reply>();
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

  ReplyType getType() {
    return this.type;
  }

  void setType(ReplyType type) {
    this.type = type;
  }

  boolean hasParent() {
    return this.parent != null;
  }

  Reply getParent() {
    return this.parent;
  }

  String getIspi() {
    Reply tmp = this;
    while (true) {
      if (tmp.getType() == ReplyType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    return tmp.ispi;
  }

  void setIspi(String ispi) {
    Reply tmp = this;
    while (true) {
      if (tmp.getType() == ReplyType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    tmp.ispi = ispi;
  }

  String getRspi() {
    Reply tmp = this;
    while (true) {
      if (tmp.getType() == ReplyType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    return tmp.rspi;
  }

  void setRspi(String rspi) {
    Reply tmp = this;
    while (true) {
      if (tmp.getType() == ReplyType.MESSAGE)
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

enum ReplyType {
  MESSAGE("message") {
    public void printReplyType(Log logger) {
      logger.info("This is a reply of a message type");
    }
  },
  PAYLOAD("payload") {
    public void printReplyType(Log logger) {
      logger.info("This is a reply of a payload type");
    }
  },
  ATTRIBUTE("attribute") {
    public void printReplyType(Log logger) {
      logger.info("This is a reply of a attribute type");
    }
  };

  private final String type;

  ReplyType(String type) {
    this.type = type;
  }

  @Override public String toString() {
    return type;
  }

  public abstract void printReplyType(Log logger);
}

enum ValueType {
  NONE(1) {
    public void printValueType(Log logger) {
      logger.info("This value type is none");
    }
  },
  INTEGER(2) {
    public void printValueType(Log logger) {
      logger.info("The value type is an integer");
    }
  },
  UINT16(3) {
    public void printValueType(Log logger) {
      logger.info("The value type is unsigned 16 bit integer");
    }
  },
  STRING(4) {
    public void printValueType(Log logger) {
      logger.info("The value type is String");
    }
  };

  private final int type;

  ValueType(int type) {
    this.type = type;
  }

  public abstract void printValueType(Log logger);
  public String getValueType() {
    return (new Integer(this.type)).toString();
  }
}
