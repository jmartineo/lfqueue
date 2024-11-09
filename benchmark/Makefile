TESTS = wfqueue wfqueue0 lcrq ccqueue msqueue faa delay cas scq scq2 scqd ncq wcq

# if using clang, please also specify -mcx16 for x86-64
CC = gcc
CFLAGS = -g -Wall -O3 -pthread -D_GNU_SOURCE -fPIC
LDLIBS = -ljemalloc -lpthread -lm

ifeq (${VERIFY}, 1)
	CFLAGS += -DVERIFY
endif

ifeq (${SANITIZE}, 1)
	CFLAGS += -fsanitize=address -fno-omit-frame-pointer
	LDLIBS += -lasan
	LDFLAGS = -fsanitize=address
endif

ifdef JEMALLOC_PATH
	LDFLAGS += -L${JEMALLOC_PATH}/lib -Wl,-rpath,${JEMALLOC_PATH}/lib
	LDLIBS += -ljemalloc
endif

SHARED_LIBS = build/libwfqueue.so build/libwfqueue0.so build/liblcrq.so build/libccqueue.so \
              build/libmsqueue.so build/libfaa.so build/libdelay.so build/libcas.so \
              build/libscq.so build/libscq2.so build/libscqd.so build/libncq.so build/libwcq.so

# Output directory
BUILD_DIR = build

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

all: $(TESTS)

# Rule for building each shared library
libwfqueue.so: CFLAGS += -DWFQUEUE
libwfqueue.so: $(BUILD_DIR)/wfqueue.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

libwfqueue0.so: CFLAGS += -DMAX_PATIENCE=0 -DWFQUEUE
libwfqueue0.so: $(BUILD_DIR)/wfqueue0.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

liblcrq.so: CFLAGS += -DLCRQ
liblcrq.so: $(BUILD_DIR)/lcrq.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o $(BUILD_DIR)/hzdptr.o $(BUILD_DIR)/xxhash.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

libccqueue.so: CFLAGS += -DCCQUEUE
libccqueue.so: $(BUILD_DIR)/ccqueue.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

libmsqueue.so: CFLAGS += -DMSQUEUE
libmsqueue.so: $(BUILD_DIR)/msqueue.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o $(BUILD_DIR)/hzdptr.o $(BUILD_DIR)/xxhash.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

libfaa.so: CFLAGS += -DFAAQ
libfaa.so: $(BUILD_DIR)/faa.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

libdelay.so: CFLAGS += -DDELAY
libdelay.so: $(BUILD_DIR)/delay.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

libcas.so: CFLAGS += -DFAAQ
libcas.so: $(BUILD_DIR)/cas.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

libscq.so: CFLAGS += -DSCQ
libscq.so: $(BUILD_DIR)/scq.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

libscq2.so: CFLAGS += -DSCQ2
libscq2.so: $(BUILD_DIR)/scq2.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

libscqd.so: CFLAGS += -DSCQD
libscqd.so: $(BUILD_DIR)/scqd.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

libncq.so: CFLAGS += -DNCQ
libncq.so: $(BUILD_DIR)/ncq.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

libwcq.so: CFLAGS += -DWCQ
libwcq.so: $(BUILD_DIR)/wcq.o $(BUILD_DIR)/harness.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

wfqueue0: CFLAGS += -DMAX_PATIENCE=0
wfqueue0.o: wfqueue.c
	$(CC) $(CFLAGS) -c -o $@ $^

haswell: CFLAGS += -DGUADALUPE_COMPACT
haswell: all

mic: CC = /usr/linux-k1om-4.7/bin/x86_64-k1om-linux-gcc
mic: CFLAGS += -DGUADALUPE_MIC_COMPACT -DLOGN_OPS=6
mic biou: $(filter-out lcrq,$(TESTS))

biou: CFLAGS += -DBIOU_COMPACT

wfqueue wfqueue0: CFLAGS += -DWFQUEUE
lcrq: CFLAGS += -DLCRQ
ccqueue: CFLAGS += -DCCQUEUE
msqueue: CFLAGS += -DMSQUEUE
faa: CFLAGS += -DFAAQ
cas: CFLAGS += -DFAAQ
delay: CFLAGS += -DDELAY
scq: CFLAGS += -DSCQ
scqd: CFLAGS += -DSCQD
scq2: CFLAGS += -DSCQ2
ncq: CFLAGS += -DNCQ
wcq: CFLAGS += -DWCQ

$(TESTS): harness.o
ifeq (${HALFHALF}, 1)
$(TESTS): halfhalf.o
else
$(TESTS): pairwise.o
endif

msqueue lcrq: hzdptr.o xxhash.o

# Object file compilation with respective flags
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TESTS) *.o $(SHARED_LIBS)
