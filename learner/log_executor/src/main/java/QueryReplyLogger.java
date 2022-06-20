import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import org.apache.commons.logging.Log;

class QueryReplyLogger {

}

class Reply {
  private Query query;
  private String orig;
  private ReplyType type;
  private List<Reply> sub = null;
  private Iterator iter = null;
  static Log logger = null;

  Reply(Query query, String result, Log logger) {
    setLogger(logger);
    this.query = query;
    this.orig = result;

    parseResult(result);

    if (this.sub != null)
      this.iter = this.sub.iterator();
  }

  Reply(Query query, String result) {
    this(query, result, null);
  }

  public String getOriginalResult() {
    return this.orig;
  }

  void setLogger(Log logger)
  {
    this.logger = logger;
  }

  void parseResult(String result) {
    logger.debug("Received result message: " + result);
  }
}

enum ReplyType {
  MESSAGE("message") {
    public void printReplyType(Log logger) {
      logger.info("This is a query of a message type");
    }
  },
  ATTRIBUTE("attribute") {
    public void printReplyType(Log logger) {
      logger.info("This is a query of a attribute type");
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
