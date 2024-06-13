# Use the following line if motorAcsMotion is built as a submodule of motor
iocshLoad("$(MOTOR)/iocsh/ACS_Motion_tcp.iocsh", "INSTANCE=ACS1,IP_ADDR=164.54.115.62,NUM_AXES=8,IDLE_POLL=0.2")

# Load motor records
dbLoadTemplate("substitutions/AcsMotion.substitutions","P=$(PREFIX)")

# Load an asyn record for debugging
dbLoadRecords("$(ASYN)/db/asynRecord.db","P=$(PREFIX),R=asyn_1,PORT=ACS1_ETH,ADDR=0,OMAX=256,IMAX=256")

# Set ERES to EFAC for each axis
doAfterIocInit("dbpf('$(PREFIX)m1.ERES','0.00005')")
doAfterIocInit("dbpf('$(PREFIX)m2.ERES','0.00005')")
doAfterIocInit("dbpf('$(PREFIX)m3.ERES','0.00005')")
doAfterIocInit("dbpf('$(PREFIX)m5.ERES','1.0')")
doAfterIocInit("dbpf('$(PREFIX)m6.ERES','1.0')")
