CREATE TABLE IF NOT EXISTS public.coustomer_dim
(
    nid numeric(13,0),
    name character varying(100),
    contact_no numeric(14,0),
    address character varying(100),
    street character varying(100),
    upazila character varying(20),
    district character varying(20),
    division character varying(20),
    coustomer_key character varying(7) PRIMARY KEY
)

TABLESPACE pg_default;

ALTER TABLE public.coustomer_dim
    OWNER to postgres;
    
COPY coustomer_dim(coustomer_key,name,contact_no,nid,address,street,upazila,district,division)
FROM '/assignment/customer_dim.csv'
DELIMITER ','
CSV HEADER;


-- store_key,location,city,upazila,district
-- S0020,"H#607,R#10,Baitul Aman Housing Society, Adabar,Dhaka-1207,Bangladesh",Dhaka,Mohammadpur,Dhaka
CREATE TABLE IF NOT EXISTS public.store_dim
(
	store_key character varying(5) PRIMARY KEY,
	location character varying(100),
	city character varying(20),
	upazila character varying(20),
	district character varying(20)
)

TABLESPACE pg_default;

ALTER TABLE public.store_dim
    OWNER to postgres;
    
COPY store_dim(store_key,location,city,upazila,district)
FROM '/assignment/store_dim.csv'
DELIMITER ','
CSV HEADER;

-- item_key,item_name,desc,unit_price,man_country,supplier,stock_quantity,unit
-- I00001,A&W Root Beer - 12 oz cans,a. Beverage - Soda,11.5,China,"CHERRY GROUP CO.,LTD",24,cans
CREATE TABLE IF NOT EXISTS public.item_dim
(
	item_key character varying(6) PRIMARY KEY,
	item_name character varying(50),
	description character varying(50),
	unit_price real,
	man_country character varying(20),
	supplier character varying(30),
	stock_quantity real,
	unit character varying(10)
)

TABLESPACE pg_default;

ALTER TABLE public.item_dim
    OWNER to postgres;
    
COPY item_dim(item_key,item_name,description,unit_price,man_country,supplier,stock_quantity,unit)
FROM '/assignment/item_dim.csv'
DELIMITER ','
CSV HEADER;

-- time_key,date,hour,day,week,month,quarter,year
-- T00001,11-02-2015 10:37,10,11,2nd Week,02,Q1,2015
CREATE TABLE IF NOT EXISTS public.time_dim
(
	time_key character varying(7) PRIMARY KEY,
	date character varying(20),
	hour smallint,
	day smallint,
	week character varying(10),
	month smallint,
	quarter character varying(2),
	year smallint
)

TABLESPACE pg_default;

ALTER TABLE public.time_dim
    OWNER to postgres;
    
COPY time_dim(time_key,date,hour,day,week,month,quarter,year)
FROM '/assignment/time_dim.csv'
DELIMITER ','
CSV HEADER;

-- payment_key,trans_type,bank_name
-- P032,online,South Bangla Agriculture and Commerce Bank Limited
CREATE TABLE IF NOT EXISTS public.trans_dim
(
	payment_key character varying(6) PRIMARY KEY,
	trans_type character varying(10),
	bank_name character varying(100)
)

TABLESPACE pg_default;

ALTER TABLE public.trans_dim
    OWNER to postgres;
    
COPY trans_dim(payment_key,trans_type,bank_name)
FROM '/assignment/trans_dim.csv'
DELIMITER ','
CSV HEADER;


CREATE TABLE IF NOT EXISTS public.fact_table
(
    coustomer_key character varying(7),
    time_key character varying(7),
    item_key character varying(6),
    store_key character varying(5),
    quantity smallint,
    unit character varying(10),
    unit_price real,
    total_price real,
    payment_key character varying(6),
    CONSTRAINT coustomer_key FOREIGN KEY (coustomer_key)
        REFERENCES public.coustomer_dim (coustomer_key) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    CONSTRAINT item_key FOREIGN KEY (item_key)
        REFERENCES public.item_dim (item_key) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    CONSTRAINT payment_key FOREIGN KEY (payment_key)
        REFERENCES public.trans_dim (payment_key) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    CONSTRAINT store_key FOREIGN KEY (store_key)
        REFERENCES public.store_dim (store_key) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    CONSTRAINT time_key FOREIGN KEY (time_key)
        REFERENCES public.time_dim (time_key) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;

ALTER TABLE public.fact_table
    OWNER to postgres;

COPY fact_table(payment_key,coustomer_key,time_key,item_key,store_key,quantity,unit,unit_price,total_price)
FROM '/assignment/fact_table.csv'
DELIMITER ','
CSV HEADER;