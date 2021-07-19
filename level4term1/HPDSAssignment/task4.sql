-- cross tabulation of item_dim by item_name and man_country
select item_name, sum(stock_quantity)
from item_dim
group by item_name;

select man_country, sum(stock_quantity)
from item_dim
group by man_country;

select sum(stock_quantity) from item_dim;

select item_name, man_country, sum(stock_quantity)
from fact_table 
group by item_name, man_country;

-- cross tabulation of fact_table by store_key and item_key
select store_key, item_key, sum(quantity)
from fact_table 
group by store_key, item_key

select store_key, sum(quantity)
from fact_table 
group by store_key;

select item_key, sum(quantity)
from fact_table 
group by item_key

select sum(quantity) from fact_table;

-- cross tabulation of fact_table by store_key and coustomer_key
select store_key, coustomer_key, sum(total_price)
from fact_table 
group by store_key, coustomer_key

select store_key, sum(total_price)
from fact_table 
group by store_key;

select coustomer_key, sum(total_price)
from fact_table 
group by coustomer_key

select sum(total_price) from fact_table;
