import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import org.apache.commons.logging.Log;

class QueryString {
  JSONParser parser;
  JSONObject orig;
  List<Query> messages;
  Iterator iter;
  static Log logger = null;

  QueryString(JSONObject queryObj, Log logger) {
    setLogger(logger);
    this.orig = queryObj;
    this.messages = new ArrayList<>();
    if (logger != null)
      logger.info("Query: " + queryObj);
    else
      System.out.println("Query: " + queryObj);
    JSONArray queryArr = (JSONArray) queryObj.get("query");
    parseQuery(queryArr);
    iter = messages.iterator();
  }

  QueryString(JSONObject queryObj) {
    this(queryObj, null);
  }

  public JSONObject getOriginalQuery() {
    return this.orig;
  }

  void setLogger(Log logger)
  {
    this.logger = logger;
  }

  void parseQuery(JSONArray queryArr) {
    logger.debug("Get query messages");
    Iterator i = queryArr.iterator();

    while (i.hasNext()) {
      JSONObject query = (JSONObject) i.next();
      logger.debug("Add one query message: " + query);
      messages.add(new Query(query, QueryType.MESSAGE, this.logger));
    }
  }

  boolean hasNextMessage() {
    return iter.hasNext();
  }

  Query getNextMessage() {
    return (Query) iter.next();
  }
}

class Query {
  private JSONObject orig;
  private String name;
  private String receiver;
  private String value = null;
  private Query parent;
  private QueryType type;
  private ValueType vtype;
  private List<Query> sub = null;
  private Iterator iter = null;
  static Log logger = null;

  private String ispi = null;
  private String rspi = null;

  Query(JSONObject query, QueryType type, Query parent, Log logger) {
    this.orig = query;
    this.type = type;
    this.parent = parent;
    this.logger = logger;

    this.receiver = (String) query.get("receiver");
    this.name = (String) query.get("name");

    logger.debug("name: " + name);
    if (query.get("value") != null) {
      String vt;
      vt = (String) query.get("type");
      if (vt.equals("integer")) {
        this.vtype = ValueType.INTEGER;
      } else if (vt.equals("uint16")) {
        this.vtype = ValueType.UINT16;
      } else if (vt.equals("string")) {
        this.vtype = ValueType.STRING;
      } else {
        this.vtype = ValueType.STRING;
      }
      this.value = query.get("value").toString();
      logger.debug("type: " + this.vtype);
      logger.debug("value: " + this.value);
    }
    
    JSONArray queryArr = (JSONArray) query.get("sub");
    if (queryArr != null) {
      Iterator i = queryArr.iterator();
      Query tmp;

      while (i.hasNext()) {
        if (this.sub == null)
          this.sub = new ArrayList<>();

        JSONObject subquery = (JSONObject) i.next();
        this.sub.add(new Query(subquery, this, this.logger));
      }

      this.iter = this.sub.iterator();
    }
  }

  Query(JSONObject query, QueryType type, Log logger) {
    this(query, type, null, logger);
  }

  Query(JSONObject query, Query parent, Log logger) {
    this(query, null, parent, logger);
    if (this.sub == null)
      this.setQueryType(QueryType.ATTRIBUTE);
    else
      this.setQueryType(QueryType.PAYLOAD);
  }

  Query(JSONObject query) {
    this(query, null, null, null);

    if (this.sub == null)
      setQueryType(QueryType.ATTRIBUTE);
    else
      setQueryType(QueryType.PAYLOAD);
  }

  QueryType getQueryType() {
    return this.type;
  }

  void setQueryType(QueryType type) {
    this.type = type;
  }

  Query getParent() {
    return this.parent;
  }

  void setParent(Query parent) {
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

  boolean getHasSubQuery() {
    return this.iter != null;
  }

  boolean getHasNextSubQuery() {
    return getHasSubQuery()? this.iter.hasNext() : false;
  }

  Query getNextSubQuery() {
    return (Query) this.iter.next();
  }

  JSONObject getOriginalQuery() {
    return orig;
  }

  String getIspi() {
    Query tmp = this;
    while (true) {
      if (tmp.getQueryType() == QueryType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    return tmp.ispi;
  }

  void setIspi(String ispi) {
    Query tmp = this;
    while (true) {
      if (tmp.getQueryType() == QueryType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    tmp.ispi = ispi;
  }

  String getRspi() {
    Query tmp = this;
    while (true) {
      if (tmp.getQueryType() == QueryType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    return tmp.rspi;
  }

  void setRspi(String rspi) {
    Query tmp = this;
    while (true) {
      if (tmp.getQueryType() == QueryType.MESSAGE)
        break;
      tmp = tmp.getParent();
    }
    tmp.rspi = rspi;
  }
}

enum QueryType {
  MESSAGE("message") {
    public void printQueryType(Log logger) {
      logger.info("This is a query of a message type");
    }
  },
  PAYLOAD("payload") {
    public void printQueryType(Log logger) {
      logger.info("This is a query of a payload type");
    }
  },
  ATTRIBUTE("attribute") {
    public void printQueryType(Log logger) {
      logger.info("This is a query of a attribute type");
    }
  };

  private final String type;

  QueryType(String type) {
    this.type = type;
  }

  @Override public String toString() {
    return type;
  }

  public abstract void printQueryType(Log logger);
}
