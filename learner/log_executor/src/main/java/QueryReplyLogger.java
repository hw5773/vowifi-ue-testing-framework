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
  private List<Reply> sub = null;
  private Iterator iter = null;
  private String ispi = null;
  private String rspi = null;
  private String name = null;
  static Log logger = null;

  Reply(Query query, Log logger) {
    setLogger(logger);
    this.query = query;
  }

  Reply(Query query) {
    this(query, null);
  }

  void setLogger(Log logger)
  {
    this.logger = logger;
  }

  ReplyType getType() {
    return this.type;
  }

  String getIspi() {
    return this.ispi;
  }

  void setIspi(String ispi) {
    this.ispi = ispi;
  }

  String getRspi() {
    return this.rspi;
  }

  void setRspi(String rspi) {
    this.rspi = rspi;
  }

  String getName() {
    return this.name;
  }

  void setName(String name) {
    this.name = name;
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
