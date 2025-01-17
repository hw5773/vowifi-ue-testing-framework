import java.io.*;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.TimeUnit;
import java.util.Iterator;
import java.util.Stack;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.commons.cli.*;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

public class Oracle {
  final static Log logger = LogFactory.getLog(Controller.class);

  public Oracle() {
    logger.info("Oracle is initialized");
  }

  public int getFunctionalOracleResult(List<QueryReplyPair> pairs) {
    logger.debug("Tagging if the behavior is deviated");

    int ret;
    Iterator i;
    QueryReplyPair pair;

    ret = 0;
    pair = pairs.get(pairs.size()-1);

    if (pair != null)
    {
      logger.debug("Query: " + pair.getQueryName() + " / Reply: " + pair.getReplyName());

      if (pair.getReplyName().contains("timeout")) {
        logger.debug("The final reply message is 'timeout', which is expected");
      } else if (isNormalSequence(pair.getQueryName(), pair.getReplyName())) {
        ret = 2;
      } else {
        ret = 1;
      }
    }

    return ret;
  }

  public int getLivenessOracleResult(List<QueryReplyPair> pairs) {
    logger.debug("Tagging if the behavior is unreliable");
    int ret;
    QueryReplyPair pair;
    String query, reply;
    Iterator i;

    ret = 0;

    i = pairs.iterator();

    while (i.hasNext()) {
      pair = (QueryReplyPair) i.next();
      query = pair.getQueryName();
      reply = pair.getReplyName();

      if (query.contains("enable_vowifi")
          && !reply.contains("ike_sa_init_request"))
        ret = 1;

      if (query.contains("ike_sa_init_response")
          && !reply.contains("ike_auth_1_request"))
        ret = 1;

      if (query.contains("ike_auth_1_response")
          && !reply.contains("ike_auth_2_request"))
        ret = 1;

      if (query.contains("ike_auth_2_response")
          && !reply.contains("ike_auth_3_request"))
        ret = 1;

      if (query.contains("ike_auth_3_response")
          && !(reply.contains("timeout") || reply.contains("register_1")))
        ret = 1;

      if (query.contains("401_unauthorized")
          && !reply.contains("register_2"))
        ret = 1;

      if (query.contains("200_ok")
          && !reply.contains("done"))
        ret = 1;

      if (reply.contains("client_error"))
      {
        ret = 2;
        break;
      }
    }

    return ret;
  }

  private boolean isNormalSequence(String query, String reply) {
    boolean ret;
    ret = true;

    if (query.contains("enable_vowifi")
        && !reply.contains("ike_sa_init_request"))
      ret = false;

    if (query.contains("ike_sa_init_response")
        && !reply.contains("ike_auth_1_request"))
      ret = false;

    if (query.contains("ike_auth_1_response")
        && !reply.contains("ike_auth_2_request"))
      ret = false;

    if (query.contains("ike_auth_2_response")
        && !reply.contains("ike_auth_3_request"))
      ret = false;

    if (query.contains("ike_auth_3_response")
        && !reply.contains("register_1"))
      ret = false;

    if (query.contains("401_unauthorized")
        && !reply.contains("register_2"))
      ret = false;

    if (query.contains("200_ok")
        && !reply.contains("done"))
      ret = false;

    return ret;
  }
}
