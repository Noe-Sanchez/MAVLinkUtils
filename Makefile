dependencies:
	python3 -m pip install -r mavlink/pymavlink/requirements.txt
	python3 -m mavlink.pymavlink.tools.mavgen --lang=C --wire-protocol=2.0 --output=mavlink/generated/include/mavlink/v2.0 mavlink/message_definitions/v1.0/common.xml
	python3 -m mavlink.pymavlink.tools.mavgen --lang=C --wire-protocol=2.0 --output=mavlink/generated/include/mavlink/v2.0 mavlink/message_definitions/v1.0/ardupilotmega.xml
	mkdir bin
decoder:
	gcc -o bin/decoder decoder.c -Wno-address-of-packed-member -I ./mavlink/generated/include/mavlink/v2.0/
