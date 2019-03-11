### Build/test container ###
FROM bbk-simple-echo-server:base as builder

COPY . /usr/src/projects
WORKDIR /usr/src/projects/build

RUN cmake ..
RUN make
RUN ctest --output-on_failure


### Deploy container ###
FROM ubuntu:latest as deploy

RUN apt-get update && apt-get install -y sqlite3 libsqlite3-dev

# TODO(!): Copy server output binary to "."
COPY --from=builder /usr/src/projects/build/bin/main .

# TODO(!): Expose some port(s)
EXPOSE 80

# Use ENTRYPOINT to specify the binary name
ENTRYPOINT ["./main"]

# Copy config file
COPY --from=builder /usr/src/projects/server_config_cloud .
# COPY --from=builder /usr/src/projects/static_src/ ../static_src/

COPY static_src/* /usr/src/projects/static_src/
COPY meme_src/* /usr/src/projects/meme_src/

# Use CMD to specify arguments to ENTRYPOINT
CMD ["server_config_cloud"]
