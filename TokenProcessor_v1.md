## TokenProcessor_v1

----

### \[Contents]

+ TokenProcessor_v1
  + TokenProcessor_v1.h
  + TokenProcessor_v1.cpp

----

### \[Description]

+ A class named "TokenProcessor" is in the "TokenProcessor_v1.h".
+ "TokenProcessor_v1.cpp" is an example for using "TokenProcessor" in the "TokenProcessor_v1.h".

----

### \[Token Types]

| Type      | Length                    | Examples                                                         |
|:---------:|:-------------------------:|:---------------------------------------------------------------- |
| STRING    | 1 ~ n characters (n >= 1) | qwer_123 _345zxcv _                                              |
| INTEGER   | 1 ~ n characters (n >= 1) | 123 456 7890 0123 00123 0 00000                                  |
| DELIMITER | 1 character               | ! " # $ % & ' ( ) * + , - . / : ; < = > ? @ \[ \ ] ^ \` { \| } ~ |
