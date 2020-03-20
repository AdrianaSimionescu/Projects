JFLAGS = -g
JC = javac
.SUFFIXES: .java .class

.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	  Benefits.java \
	  Family.java \
	  Group.java \
	  Passenger.java \
	  Person.java \
	  PriorityQ.java \
	  Single.java \
	  Test.java \
	  Ticket.java 

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
