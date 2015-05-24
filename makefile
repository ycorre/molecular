
all:
	(cd sources/make/; make)
	cp -f sources/make/molecular .
	
clean:
	(cd sources/make/; make clean)

