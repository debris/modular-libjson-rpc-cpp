#!/bin/bash

FILENAME=$1
NAME=$2
INCLUDE_NAME="\"..\\/ModularServer.h\""

echo "fixing ${NAME} in ${FILENAME}"

sed -e s/include\ \<jsonrpccpp\\/server\.h\>/include\ ${INCLUDE_NAME}/g \
    -e s/public\ jsonrpc::AbstractServer\<${NAME}\>/public\ ServerInterface\<${NAME}\>/g \
    -e s/${NAME}\(jsonrpc::AbstractServerConnector\ \&conn,\ jsonrpc::serverVersion_t\ type\ =\ jsonrpc::JSONRPC_SERVER_V2\)\ :\ jsonrpc::AbstractServer\<${NAME}\>\(conn,\ type\)/${NAME}\(\)/g \
    < ${FILENAME} > ${NAME}Module.h

