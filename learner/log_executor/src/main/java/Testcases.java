import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import org.apache.commons.logging.Log;

class Testcases {
  JSONParser parser;
  JSONObject orig;
  List<Testcase> messages;
  Iterator iter;
  static Log logger = null;

  Testcases(JSONObject testcaseObj, Log logger) {
    setLogger(logger);
    this.orig = testcaseObj;
    this.messages = new ArrayList<>();
    if (logger != null)
      logger.info("Testcase: " + testcaseObj);
    else
      System.out.println("Testcase: " + testcaseObj);
    JSONArray testcaseArr = (JSONArray) testcaseObj.get("testcase");
    parseTestcases(testcaseArr);
    iter = messages.iterator();
  }

  Testcases(JSONObject testcaseObj) {
    this(testcaseObj, null);
  }

  public JSONObject getOriginalTestcase() {
    return this.orig;
  }

  void setLogger(Log logger)
  {
    this.logger = logger;
  }

  void parseTestcases(JSONArray testcaseArr) {
    logger.debug("Get testcase messages");
    Iterator i = testcaseArr.iterator();

    while (i.hasNext()) {
      JSONObject testcase = (JSONObject) i.next();
      logger.debug("Add one testcase message: " + testcase);
      messages.add(new Testcase(testcase, TestcaseType.MESSAGE, this.logger));
    }
  }

  boolean hasNextMessage() {
    return iter.hasNext();
  }

  Testcase getNextMessage() {
    return (Testcase) iter.next();
  }
}

class Testcase {
  private JSONObject orig;
  private String name;
  private String receiver;
  private String value = null;
  private Testcase parent;
  private TestcaseType type;
  private ValueType vtype;
  private List<Testcase> sub = null;
  private Iterator iter = null;
  static Log logger = null;

  private String ispi = null;
  private String rspi = null;

  Testcase(JSONObject testcase, TestcaseType type, Testcase parent, Log logger) {
    this.orig = testcase;
    this.type = type;
    this.parent = parent;
    this.logger = logger;

    this.receiver = (String) testcase.get("receiver");
    this.name = (String) testcase.get("name");

    logger.debug("name: " + name);
    if (testcase.get("value") != null) {
      String vt;
      vt = (String) testcase.get("type");
      if (vt.equals("integer")) {
        this.vtype = ValueType.INTEGER;
      } else if (vt.equals("uint16")) {
        this.vtype = ValueType.UINT16;
      } else if (vt.equals("string")) {
        this.vtype = ValueType.STRING;
      } else {
        this.vtype = ValueType.STRING;
      }
      this.value = testcase.get("value").toString();
      logger.debug("type: " + this.vtype);
      logger.debug("value: " + this.value);
    }
    
    JSONArray testcaseArr = (JSONArray) testcase.get("sub");
    if (testcaseArr != null) {
      Iterator i = testcaseArr.iterator();
      Testcase tmp;

      while (i.hasNext()) {
        if (this.sub == null)
          this.sub = new ArrayList<>();

        JSONObject subtestcase = (JSONObject) i.next();
        this.sub.add(new Testcase(subtestcase, this, this.logger));
      }

      this.iter = this.sub.iterator();
    }
  }

  Testcase(JSONObject testcase, TestcaseType type, Log logger) {
    this(testcase, type, null, logger);
  }

  Testcase(JSONObject testcase, Testcase parent, Log logger) {
    this(testcase, null, parent, logger);
    if (this.sub == null)
      this.setTestcaseType(TestcaseType.ATTRIBUTE);
    else
      this.setTestcaseType(TestcaseType.PAYLOAD);
  }

  Testcase(JSONObject testcase) {
    this(testcase, null, null, null);

    if (this.sub == null)
      setTestcaseType(TestcaseType.ATTRIBUTE);
    else
      setTestcaseType(TestcaseType.PAYLOAD);
  }

  TestcaseType getTestcaseType() {
    return this.type;
  }

  void setTestcaseType(TestcaseType type) {
    this.type = type;
  }

  Testcase getParent() {
    return this.parent;
  }

  void setParent(Testcase parent) {
    this.parent = parent;
  }

  String getReceiver() {
    return receiver;
  }

  void setReceiver(String receiver) {
    this.receiver = receiver;
  }

  ValueType getValueType() {
    return this.vtype;
  }

  void setValueType(ValueType vtype) {
    this.vtype = vtype;
  }

  String getName() {
    return this.name;
  }

  void setName(String command) {
    this.name = name;
  }

  boolean hasValue() {
    return this.value != null;
  }

  String getValue() {
    return this.value;
  }

  void setValue(String value) {
    this.value = value;
  }

  boolean getHasSubTestcase() {
    return this.iter != null;
  }

  boolean getHasNextSubTestcase() {
    return getHasSubTestcase()? this.iter.hasNext() : false;
  }

  Testcase getNextSubTestcase() {
    return (Testcase) this.iter.next();
  }

  JSONObject getOriginalTestcase() {
    return orig;
  }

  String getIspi() {
    Testcase tmp = this;
    while (true) {
      if (tmp.getTestcaseType() == TestcaseType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    return tmp.ispi;
  }

  void setIspi(String ispi) {
    Testcase tmp = this;
    while (true) {
      if (tmp.getTestcaseType() == TestcaseType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    tmp.ispi = ispi;
  }

  String getRspi() {
    Testcase tmp = this;
    while (true) {
      if (tmp.getTestcaseType() == TestcaseType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    return tmp.rspi;
  }

  void setRspi(String rspi) {
    Testcase tmp = this;
    while (true) {
      if (tmp.getTestcaseType() == TestcaseType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    tmp.rspi = rspi;
  }
}

enum TestcaseType {
  MESSAGE("message") {
    public void printTestcaseType(Log logger) {
      logger.info("This is a testcase of a message type");
    }
  },
  PAYLOAD("payload") {
    public void printTestcaseType(Log logger) {
      logger.info("This is a testcase of a payload type");
    }
  },
  ATTRIBUTE("attribute") {
    public void printTestcaseType(Log logger) {
      logger.info("This is a testcase of a attribute type");
    }
  };

  private final String type;

  TestcaseType(String type) {
    this.type = type;
  }

  @Override public String toString() {
    return type;
  }

  public abstract void printTestcaseType(Log logger);
}
