FROM ubuntu:18.04
RUN apt update -y
RUN apt install apache2 -y
RUN apt install g++ -y
RUN apt install libcgicc3 -y
RUN apt install libcgicc-dev -y
RUN apt install make -y

copy index.html /var/www/html/
copy conf/apache2.conf /etc/apache2/apache2.conf
copy conf/cgid.conf /etc/apache2/mods-enabled/
copy conf/cgid.load /etc/apache2/mods-enabled/
copy conf/cgi.load /etc/apache2/mods-enabled/
copy src/serverRequest.js /var/www/html/src/serverRequest.js
copy src/index.css /var/www/html/src/index.css

copy src/Exo2-Bold.woff /var/www/html/src/Exo2-Bold.woff
copy src/Exo2-Bold.woff2 /var/www/html/src/Exo2-Bold.woff2
copy src/Exo2-ExtraBold.woff /var/www/html/src/Exo2-ExtraBold.woff
copy src/Exo2-ExtraBold.woff2 /var/www/html/src/Exo2-ExtraBold.woff2
copy src/Exo2-Regular.woff /var/www/html/src/Exo2-Regular.woff
copy src/Exo2-Regular.woff2 /var/www/html/src/Exo2-Regular.woff2

copy src/1.png /var/www/html/src/1.png
copy src/2.png /var/www/html/src/2.png
copy src/3.png /var/www/html/src/3.png
copy src/4.png /var/www/html/src/4.png
copy src/arrow.png /var/www/html/src/arrow.png
copy src/favicon.ico /var/www/html/src/favicon.ico
copy src/logo.png /var/www/html/src/logo.png
copy src/main.jpg /var/www/html/src/main.jpg
copy src/result.png /var/www/html/src/result.png

copy src/particles.js /var/www/html/src/particles.js
copy src/select.js /var/www/html/src/select.js
copy src/ibg.js /var/www/html/src/ibg.js
copy src/app.js /var/www/html/src/app.js

RUN mkdir /var/www/cgi-bin
copy Makefile /var/www/cgi-bin/
copy script.cpp /var/www/cgi-bin/
copy db.txt /var/www/cgi-bin/db.txt
WORKDIR /var/www/cgi-bin
RUN make
RUN chmod 755 script.cgi
RUN rm Makefile script.cpp
CMD apache2ctl -D FOREGROUND