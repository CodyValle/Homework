/**************************************************************
* SQL script file used to create the database named MedicalData
* and load its tables from text files.
**************************************************************/

DROP DATABASE IF EXISTS MedicalData;

CREATE DATABASE MedicalData;

USE MedicalData;

/** Create the Demo table **/
create table demo08Q1
(
ISR MEDIUMINT UNSIGNED NOT NULL,
CASE_NUM INT NOT NULL,
I_F_COD ENUM('I','F') NOT NULL,
FOLL_SEQ varchar(255),
IMAGE varchar(255) NOT NULL,
EVENT_DT char(8),
MFR_DT char(8),
FDA_DT char(8),
REPT_COD char(3),
MFR_NUM varchar(255),
MFR_SNDR varchar(255),
AGE SMALLINT DEFAULT NULL,
AGE_COD enum('DEC','YR','MON','WK','DY','HR'),
GNDR_COD enum('UNK','M','F','NS'),
E_SUB enum('Y','N'),
WT SMALLINT,
WT_COD enum('KG','LBS','GMS'),
REPT_DT char(8),
OCCP_COD enum('MD','PH','OT','LW','CN'),
DEATH_DT char(8),
TO_MFR enum('Y','N'),
CONFID enum('Y','N'),
REPORTER_COUNTRY varchar(255)
);
drop table demo08q1;
load data local infile "C:/Users/cvalle/Desktop/DEMO08Q1.TXT"
into table demo08q1
fields terminated by '$'
(ISR, CASE_NUM, I_F_COD, FOLL_SEQ, IMAGE, EVENT_DT, MFR_DT,FDA_DT,
REPT_COD, MFR_NUM, MFR_SNDR, @vAGE, @vAGE_COD, GNDR_COD,E_SUB, WT, WT_COD,
REPT_DT, OCCP_COD, DEATH_DT, TO_MFR, CONFID, REPORTER_COUNTRY)
set
AGE = nullif(@vAGE, ''),
AGE_COD = nullif(@vAGE_COD, '');

/** Create the Drug table **/
create table drug08Q1
(
ISR MEDIUMINT UNSIGNED NOT NULL,
DRUG_SEQ INT UNSIGNED NOT NULL,
ROLE_COD ENUM('PS','SS','C','I'),
DRUGNAME varchar(255) NOT NULL,
VAL_VBM enum('1','2') NOT NULL,
ROUTE varchar(255),
DOSE_VBM varchar(255),
DECHAL enum('Y','N','U','D'),
RECHAL enum('Y','N','U','D'),
LOT_NUM varchar(255),
EXP_DT DATE,
NDA_NUM MEDIUMINT UNSIGNED
);

load data local infile "C:/Users/cvalle/Desktop/DRUG08Q1.TXT"
into table drug08q1
fields terminated by '$'
(ISR, DRUG_SEQ, ROLE_COD, DRUGNAME, VAL_VBM, @vROUTE,
@vDOSE_VBM, @vDECHAL, @vRECHAL, @vLOT_NUM, @vEXP_DT, @vNDA_NUM)
set
ROUTE = nullif(@vROUTE, ''),
DOSE_VBM = nullif(@vDOSE_VBM, ''),
DECHAL = nullif(@vDECHAL, ''),
RECHAL = nullif(@vRECHAL, ''),
LOT_NUM = nullif(@vLOT_NUM, ''),
EXP_DT = case when @vEXP_DT = '' then null else STR_TO_DATE(@vEXP_DT, '%Y%m%d') end,
NDA_NUM = nullif(@vNDA_NUM, '');

/** Create the Reaction table **/
create table reac08q1
(
ISR MEDIUMINT UNSIGNED NOT NULL,
PT varchar(255) NOT NULL
);

load data local infile "C:/Users/cvalle/Desktop/REAC08Q1.TXT"
into table reac08q1
fields terminated by '$';