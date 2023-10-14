create user 'pcscf'@'localhost' identified by 'heslo';
grant delete,insert,select,update on pcscf.* to 'pcscf'@'localhost';
create user 'scscf'@'localhost' identified by 'heslo';
grant delete,insert,select,update on scscf.* to 'scscf'@'localhost';
create user 'icscf'@'localhost' identified by 'heslo';
grant delete,insert,select,update on icscf.* to 'icscf'@'localhost';
create user 'provisioning'@'localhost' identified by 'provi';
grant delete,insert,select,update on icscf.* to 'provisioning'@'localhost';
flush privileges;

use icscf;
insert into nds_trusted_domains values (1, 'ims.mnc260.mcc310.3gppnetwork.org');
insert into s_cscf values (1, 'First and only S-CSCF', 'sip:scscf.ims.mnc260.mcc310.3gppnetwork.org:6060');
insert into s_cscf_capabilities values (1,1,0),(2,1,1);

