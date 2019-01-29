### coverage report generating container ###
FROM bbk-simple-echo-server:base as coverage

COPY . /usr/src/project
WORKDIR /usr/src/project/build_coverage

RUN cmake -DCMAKE_BUILD_TYPE=Coverage ..
RUN make coverage

### Deploy container ###
FROM ubuntu:latest as deploy

# TODO(!): Copy server output binary to "."
COPY --from=coverage /usr/src/project/build_coverage/bin/main .

