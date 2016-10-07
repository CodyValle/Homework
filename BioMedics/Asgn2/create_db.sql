/**************************************************************
* SQL script file used reate the database named PatientDatabase
* and all of its tables
**************************************************************/

DROP DATABASE IF EXISTS PatientDatabase;

CREATE DATABASE PatientDatabase;

USE PatientDatabase;

CREATE TABLE Patient (
  Patient_ID INT NOT NULL,
  Patient_Name VARCHAR(50),
  Date_of_Birth DATE,
  Height INT,
  Weight INT,
  PRIMARY KEY(Patient_ID) 
);

INSERT INTO Patient 
  (Patient_ID, Patient_Name, Date_of_Birth, Height, Weight)
VALUES 
  (123, 'Milhouse', '1979-11-02', 63, 132),
  (142, 'Bart', '1965-04-09', 71, 183),
  (456, 'Ralph', '1974-10-21', 67, 201),
  (857, 'Lisa', '1983-02-23', 67, 147);

create table Medicine
(
Med_ID varchar(255) NOT NULL,
Patient_ID int NOT NULL,
Med_Name varchar(255),
Start_Date date,
Med_Price float,
primary key (Med_ID),
foreign key Patient_ID(Patient_ID) references Patient(Patient_ID)
);

insert into Medicine values
('ALCOHOL01', 142, 'ALCOHOL PREP PAD', '1998-09-12',5.99),
('ALCOHOL02', 142, 'ALCOHOL 1.4% LIQUID', '2000-05-03',8.99),
('ALCOHOL03', 142, 'ALCOHOL PREP PAD', '2006-11-08',11.99),
('ASPIRIN01', 857, 'ASPIRIN 325MG TAB', '2006-06-08',4.99),
('CISAPRIDE01', 857, 'CISAPRIDE 5MG TAB', '2000-07-25',3.49),
('CISAPRIDE02', 857, 'CISAPRIDE 10MG TAB', '2004-03-18',3.49),
('CISAPRIDE03', 857, 'CISAPRIDE MONOHYDRATE 5ML SUSP', '2004-12-01',4.49);

