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
      messages.add(new Query(query, QueryType.MESSAGE));
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
  private QueryType type;
  private List<Query> sub = null;
  private Iterator iter = null;
  static Log logger = null;

  Query(JSONObject query, QueryType type, Log logger) {
    this.orig = query;
    this.type = type;

    this.receiver = (String) query.get("receiver");
    this.name = (String) query.get("name");
    this.logger = logger;
    
    JSONArray queryArr = (JSONArray) query.get("sub");

    if (queryArr != null) {
      Iterator i = queryArr.iterator();
      this.sub = new ArrayList<>();

      while (i.hasNext()) {
        JSONObject subquery = (JSONObject) i.next();
        this.sub.add(new Query(subquery));
      }

      this.iter = this.sub.iterator();
    }
  }

  Query(JSONObject query, QueryType type) {
    this(query, type, null);
  }

  Query(JSONObject query) {
    this(query, null);
    setQueryType(QueryType.ATTRIBUTE);
  }

  QueryType getQueryType() {
    return type;
  }

  void setQueryType(QueryType type) {
    this.type = type;
  }

  String getReceiver() {
    return receiver;
  }

  void setReceiver(String receiver) {
    this.receiver = receiver;
  }

  String getName() {
    return this.name;
  }

  void setName(String command) {
    this.name = name;
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
}

enum QueryType {
  MESSAGE("message") {
    public void printQueryType(Log logger) {
      logger.info("This is a query of a message type");
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
