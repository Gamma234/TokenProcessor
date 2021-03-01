## TokenProcessor_v2

----

### \[Contents]

+ TokenProcessor_v2
  + TokenProcessor.h
  + TokenProcessor_v2.cpp

----

### \[Description]

+ A class named "TokenProcessor" is in the "TokenProcessor.h".
+ "TokenProcessor_v2.cpp" is an example for using "TokenProcessor" in the "TokenProcessor.h".

----

### \[Token Types]

| Type      | Length                    | Examples                                                                          |
|:--------- |:-------------------------:|:--------------------------------------------------------------------------------- |
| STRING    | 1 ~ n characters (n >= 1) | qwer_123 _345zxcv _                                                               |
| INTEGER   | 1 ~ n characters (n >= 1) | 123 456 7890 0123 00123 0 00000                                                   |
| DOUBLE    | 2 ~ n characters (n >= 2) | 1.4 12.45 0.0 0. .0 0.1 1. .4                                                     |
| DELIMITER | 1 character               | ! " # $ % & ' ( ) * + , - . / : ; < = > ? @ \[ \ ] ^ \` { \| } ~                  |
| DELIMITER | 2 characters              | += ++ -= -- -> \*= \*/ /= // /\* %= <= << >= >> != == && &= \|\| \|= :: ## .\* ^= |
| DELIMITER | 3 characters              | ->* <<= >>=                                                                       |
