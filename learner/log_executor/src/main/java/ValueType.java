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
  public String getValueTypeAsInteger() {
    return (new Integer(this.type)).toString();
  }
}