
# Docker base lambda functions

lots of issues around mac platform being different from lambda runtime.

Hopefully this docker build command fixes it:

```
docker build --platform linux/amd64 -t 901171032901.dkr.ecr.us-west-1.amazonaws.com/sailface-cloud:latest .

```

Login to AWS container registry
```
aws ecr get-login-password --region  us-west-1 | docker login --username AWS --password-stdin 901171032901.dkr.ecr.us-west-1.amazonaws.com
```

Push the new docker container
```
docker push 901171032901.dkr.ecr.us-west-1.amazonaws.com/sailface-cloud:latest
```


## Test locally

### Start lambda emulator

```
 docker run -it -p 9000:8080 901171032901.dkr.ecr.us-west-1.amazonaws.com/sailface-cloud:latest
```

### Hit the endpoint

curl -XPOST "http://localhost:9000/2015-03-31/functions/function/invocations" -d @example_request.json
