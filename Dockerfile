FROM base_prot:latest as min-stage

ARG NBUILDCORES=3
#--build-arg NBUILDCORES=12
COPY ./ /opt/prot
WORKDIR /opt/prot/
RUN ./build.sh
WORKDIR /opt/prot/build


FROM min-stage as production-stage
COPY  --from=min-stage /opt/prot/build/api-server /opt/prot/build/api-server
COPY  --from=min-stage /opt/prot/build/test_prot /opt/prot/build/test_prot
COPY  --from=min-stage /opt/prot/build/liblprot.a /opt/prot/build/liblprot.a
COPY  --from=min-stage /opt/prot/source /opt/prot/source
COPY  --from=min-stage /opt/prot/rest_server/impl /opt/prot/rest_server/impl
COPY  --from=min-stage /opt/prot/rest_server/model /opt/prot/rest_server/model
COPY  --from=min-stage /opt/prot/rest_server/api /opt/prot/rest_server/api
COPY  --from=min-stage /opt/prot/api /opt/prot/api
COPY  --from=min-stage /opt/prot/.vscode /opt/prot/.vscode
COPY  --from=min-stage /opt/prot/forms /opt/prot/forms

WORKDIR /opt/prot/build
ENTRYPOINT [ "/bin/bash" ]