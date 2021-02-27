FROM centos
COPY mylab1.sh .
RUN chmod ugo+x	mylab1.sh
CMD ./mylab1.sh
