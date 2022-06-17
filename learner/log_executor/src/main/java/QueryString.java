import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import org.apache.commons.logging.Log;

class QueryString {
  JSONParser parser;
  JSONObject orig;
  List<Query> messages;
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
  }
}

class Query {
  private JSONObject orig;
  private String name;
  private String receiver;
  private QueryType type;
  private List<Query> sub;

  Query(JSONObject query, QueryType type) {
    this.orig = query;
    this.type = type;
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
