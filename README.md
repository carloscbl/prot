[![Codacy Badge](https://api.codacy.com/project/badge/Grade/2d4567f7bacd461aaec359bcb6e2c54c)](https://www.codacy.com/manual/carloscbl/prot?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=carloscbl/prot&amp;utm_campaign=Badge_Grade)

# Prot
Is a C++ core of task scheduler, with support for cloud apps

Powered by a engine parser of json forms, that provides the set up, pipeline and question/answer front end to custom the auto-task collection called "taskstories".

## Quick - Setup
Use docker and docker compose to compile
```bash
docker build --rm --build-arg NBUILDCORES=$CORES -f "prot/Dockerfile.production" -t prot:latest "prot"
```

## License
It is unlicensed, you can only use it to learn from the code. Reach me for commercial traits or secondary uses.
Working to provide a commercial solution in the future .
