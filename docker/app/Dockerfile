FROM base-image:latest

COPY . /app
WORKDIR /app

RUN cmake -B build
RUN cmake --build ./build --config Release

CMD ["./build/test"]