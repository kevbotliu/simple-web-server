### Build/test container ###
FROM bbk-simple-echo-server:base as builder

COPY . /usr/src/project
WORKDIR /usr/src/project/build

RUN cmake ..
RUN make
RUN ctest --output-on_failure


### Deploy container ###
FROM ubuntu:latest as deploy

# TODO(!): Copy server output binary to "."
COPY --from=builder /usr/src/project/build/bin/main .

# TODO(!): Expose some port(s)
EXPOSE 80

# Use ENTRYPOINT to specify the binary name
ENTRYPOINT ["./main"]

# Copy config file
COPY --from=builder /usr/src/project/server_config_cloud .
# COPY --from=builder /usr/src/project/static_src/ ../static_src/

COPY static_src/* /usr/src/project/static_src/
COPY meme_src/* /usr/src/project/meme_src/

# Use CMD to specify arguments to ENTRYPOINT
CMD ["server_config_cloud"]
