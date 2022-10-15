FROM python:3.9

RUN mkdir /app
WORKDIR /app

COPY rock7_public.key .

COPY requirements.txt .

RUN pip install -r requirements.txt