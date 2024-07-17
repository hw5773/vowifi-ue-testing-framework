# Environment

## Gradle version (4.4.1)
 - 4.4.1: sudo apt-get install gradle

## Java version 1.8
 - JAVA_HOME

## Compile
 - gradle assemble

## Execution
 - java -cp build/libs -jar build/libs/main_controller.jar --config <configuration file> --file <path of the testcase to be executed>
 - e.g.,) java -cp build/libs -jar build/libs/main_controller.jar --config ../config/310210.properties --file ../../testcases/epdg.json
