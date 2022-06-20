import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.lang.*;

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
    int idx, type, len;
    long ispi, rspi;
    byte rcvd[] = result.getBytes();
    
    idx = 0;
    len = result.length();

    type = (int) rcvd[idx++];
    len -= 1;
    logger.debug("Type: " + type);

    ispi = parseSPI(rcvd, idx);
    idx += 8; len -= 8;
    rspi = parseSPI(rcvd, idx);
    idx += 8; len -= 8;

    logger.debug("Initiator SPI: " + Long.toUnsignedString(ispi));
    logger.debug("Responder SPI: " + Long.toUnsignedString(rspi));

    if (len > 0)
      logger.debug("Name: " + result.substring(idx));
  }

  long parseSPI(byte spi[], int idx) {
    long ret;

    ret = 0;

    for (int i=idx; i<idx+8; i++) {
      ret = (ret << 8) + (spi[i] & 0xff);
    }

    return ret;
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
