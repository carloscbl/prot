#!/bin/bash
docker run --rm -u $(id -u ${USER}):$(id -g ${USER}) -v ${PWD}:/local openapitools/openapi-generator-cli generate \
    -i /local/api/prot.v1.yaml \
    -g cpp-pistache-server \
    -o /local/rest_server