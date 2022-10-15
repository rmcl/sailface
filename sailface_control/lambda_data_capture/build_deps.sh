#!/bin/bash
export PKG_DIR="vendor"
rm -rf ${PKG_DIR} && mkdir -p ${PKG_DIR}
docker run --rm -v $(pwd):/foo -w /foo public.ecr.aws/sam/build-python3.9:1.55.0-20220818005345 \
  pip install -r aws_requirements.txt -t ${PKG_DIR}