import org.apache.commons.logging.Log;

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
  UINT8(3) {
    public void printValueType(Log logger) {
      logger.info("The value type is unsigned 8 bit integer");
    }
  },
  UINT8H(4) {
    public void printValueType(Log logger) {
      logger.info("The value type is unsigned 8 bit integer");
    }
  },
  UINT16(5) {
    public void printValueType(Log logger) {
      logger.info("The value type is unsigned 16 bit integer");
    }
  },
  UINT16H(6) {
    public void printValueType(Log logger) {
      logger.info("The value type is unsigned 16 bit integer");
    }
  },
  UINT32(7) {
    public void printValueType(Log logger) {
      logger.info("The value type is unsigned 32 bit integer");
    }
  },
  UINT32H(8) {
    public void printValueType(Log logger) {
      logger.info("The value type is unsigned 32 bit integer");
    }
  },
  UINT64(9) {
    public void printValueType(Log logger) {
      logger.info("The value type is unsigned 64 bit integer");
    }
  },
  UINT64H(10) {
    public void printValueType(Log logger) {
      logger.info("The value type is unsigned 64 bit integer");
    }
  },
  STRING(11) {
    public void printValueType(Log logger) {
      logger.info("The value type is String");
    }
  };

  private final int type;

  ValueType(int type) {
    this.type = type;
  }

  public abstract void printValueType(Log logger);
  public String getValueTypeAsInteger() {
    return (new Integer(this.type)).toString();
  }
}
