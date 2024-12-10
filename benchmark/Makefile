# Executable and shared library targets
TESTS = wfqueue wfqueue0 lcrq ccqueue msqueue faa delay cas scq scq2 scqd ncq wcq
SHARED_LIBS = build/libwfqueue.so build/libwfqueue0.so build/liblcrq.so build/libccqueue.so \
              build/libmsqueue.so build/libfaa.so build/libcas.so \
              build/libscq.so build/libscq2.so build/libscqd.so build/libncq.so build/libwcq.so

# if using clang, please also specify -mcx16 for x86-64
CC = gcc
CFLAGS = -g -Wall -O3 -pthread -D_GNU_SOURCE -fPIC
LDLIBS = -ljemalloc -lpthread -lm

# Conditional compilation flags
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

# Output directory
BUILD_DIR = build

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

CFLAGS += -I../../../../../benchmarks/C/


all: $(TESTS) $(SHARED_LIBS)

shared: $(SHARED_LIBS)

# Rule for each shared library
$(BUILD_DIR)/libwfqueue.so: CFLAGS += -DWFQUEUE
$(BUILD_DIR)/libwfqueue.so: $(BUILD_DIR)/wfqueue.o $(BUILD_DIR)/harness_wrapper.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/libwfqueue0.so: CFLAGS += -DMAX_PATIENCE=0 -DWFQUEUE
$(BUILD_DIR)/libwfqueue0.so: $(BUILD_DIR)/wfqueue.o $(BUILD_DIR)/harness_wrapper.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/liblcrq.so: CFLAGS += -DLCRQ
$(BUILD_DIR)/liblcrq.so: $(BUILD_DIR)/lcrq.o $(BUILD_DIR)/harness_wrapper.o $(BUILD_DIR)/pairwise.o $(BUILD_DIR)/hzdptr.o $(BUILD_DIR)/xxhash.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/libccqueue.so: CFLAGS += -DCCQUEUE
$(BUILD_DIR)/libccqueue.so: $(BUILD_DIR)/ccqueue.o $(BUILD_DIR)/harness_wrapper.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/libmsqueue.so: CFLAGS += -DMSQUEUE
$(BUILD_DIR)/libmsqueue.so: $(BUILD_DIR)/msqueue.o $(BUILD_DIR)/harness_wrapper.o $(BUILD_DIR)/pairwise.o $(BUILD_DIR)/hzdptr.o $(BUILD_DIR)/xxhash.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/libfaa.so: CFLAGS += -DFAAQ
$(BUILD_DIR)/libfaa.so: $(BUILD_DIR)/faa.o $(BUILD_DIR)/harness_wrapper.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/libcas.so: CFLAGS += -DFAAQ
$(BUILD_DIR)/libcas.so: $(BUILD_DIR)/cas.o $(BUILD_DIR)/harness_wrapper.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/libscq.so: CFLAGS += -DSCQ
$(BUILD_DIR)/libscq.so: $(BUILD_DIR)/scq.o $(BUILD_DIR)/harness_wrapper.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/libscq2.so: CFLAGS += -DSCQ2
$(BUILD_DIR)/libscq2.so: $(BUILD_DIR)/scq2.o $(BUILD_DIR)/harness_wrapper.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/libscqd.so: CFLAGS += -DSCQD
$(BUILD_DIR)/libscqd.so: $(BUILD_DIR)/scqd.o $(BUILD_DIR)/harness_wrapper.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/libncq.so: CFLAGS += -DNCQ
$(BUILD_DIR)/libncq.so: $(BUILD_DIR)/ncq.o $(BUILD_DIR)/harness_wrapper.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/libwcq.so: CFLAGS += -DWCQ
$(BUILD_DIR)/libwcq.so: $(BUILD_DIR)/wcq.o $(BUILD_DIR)/harness_wrapper.o $(BUILD_DIR)/pairwise.o | $(BUILD_DIR)
	$(CC) -shared -o $@ $^ $(LDLIBS)

# Executable rules
wfqueue0: CFLAGS += -DMAX_PATIENCE=0
wfqueue0.o: wfqueue.c
	$(CC) $(CFLAGS) -c -o $@ $^

haswell: CFLAGS += -DGUADALUPE_COMPACT
haswell: all

mic: CC = /usr/linux-k1om-4.7/bin/x86_64-k1om-linux-gcc
mic: CFLAGS += -DGUADALUPE_MIC_COMPACT -DLOGN_OPS=6
mic biou: $(filter-out lcrq,$(TESTS))

biou: CFLAGS += -DBIOU_COMPACT

# Feature flags for executables
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

# Dependency rules for tests
$(TESTS): harness.o
ifeq (${HALFHALF}, 1)
$(TESTS): halfhalf.o
else
$(TESTS): pairwise.o
endif

msqueue lcrq: hzdptr.o xxhash.o

# Object file compilation rule with respective flags
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean rule
clean:
	rm -f $(TESTS) *.o $(SHARED_LIBS)
	rm -rf $(BUILD_DIR)