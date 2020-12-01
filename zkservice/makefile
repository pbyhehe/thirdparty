
PWD_PATH=$(shell pwd)

PROTOC_LIB='http://storage.jd.local/psearch-thirdparty/libzkservice.a'

cloudpath=0
path=$(PWD_PATH)

target_lib=$(path)/lib/libzkservice.a

all : target_lib

target_lib:
ifeq (,$(wildcard $(target_lib)))
	mkdir -p $(path)/lib
	wget -q $(PROTOC_LIB) -O ${target_lib}
endif
