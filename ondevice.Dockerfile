FROM python:3.9

RUN mkdir /app
WORKDIR /app

COPY . .

RUN pip install nose esp32-machine-emulator