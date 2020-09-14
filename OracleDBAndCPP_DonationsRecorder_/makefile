#
## CSCI 370: Sample Makefile for OCCI compilation
#

G = g++-4.9
CFLAGS = -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
LFLAGS = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei
all: taxReceipt recordDonation

taxReceipt.o: taxReceipt.cpp
	$(G) -c $(CFLAGS) taxReceipt.cpp

recordDonation.o: recordDonation.cpp
	$(G) -c $(CFLAGS) recordDonation.cpp

taxReceipt: taxReceipt.o
	$(G) $(LFLAGS) -o taxReceipt taxReceipt.o

recordDonation: recordDonation.o
	$(G) $(LFLAGS) -o recordDonation recordDonation.o


#exampleUnprepared.o: exampleUnprepared.cpp
#	$(G) -c $(CFLAGS) exampleUnprepared.cpp

#exampleU: exampleUnprepared.o
#	$(G) $(LFLAGS) -o exampleU exampleUnprepared.o

clean:
	rm -f *.o exampleP exampleU


