# create postgre container
docker run --name postgresql-container -p 5432:5432 -v $(pwd)/:/assignment -e POSTGRES_PASSWORD=somePassword -d postgres

# create pgadmin container
docker run -d -p 5050:80 --name pgadmin-container -e PGADMIN_DEFAULT_EMAIL=mirmahathir1@gmail.com -e PGADMIN_DEFAULT_PASSWORD=somePassword dpage/pgadmin4

# enter into postgres container
docker exec -it postgresql-container bash
