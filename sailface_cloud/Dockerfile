FROM public.ecr.aws/lambda/python:3.9-x86_64

# Copy emulator so we can test locally if desired
# https://github.com/aws/aws-lambda-runtime-interface-emulator/releases/latest/download/aws-lambda-rie
COPY ./entry_script.sh /entry_script.sh
RUN chmod +x /entry_script.sh
ADD vendor/aws-lambda-rie /usr/local/bin/aws-lambda-rie
RUN chmod +x /usr/local/bin/aws-lambda-rie

RUN ln -s /var/lang/bin/python3.9 /usr/local/bin/python

COPY requirements.txt  .
RUN  pip3 install -r requirements.txt --target "${LAMBDA_TASK_ROOT}"

COPY sailface_cloud "${LAMBDA_TASK_ROOT}"/sailface_cloud

COPY rock7_public.key "${LAMBDA_TASK_ROOT}"


CMD [ "sailface_cloud.lambda_data_capture.lambda_handler" ]
ENTRYPOINT [ "/entry_script.sh" ]