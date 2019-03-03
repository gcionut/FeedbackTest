# FeedbackTest

Am folosit biblioteca cpprestsdk pentru a defeni un HTTPEndPoint. 
Baza de date folosita este MySql
Tabele:
  session
  user
  feedback
 Query
  select * from feedback where rate = ?
  `insert into  feedback( userid,idsession,rate,feedback) VALUES ( ?,?,?,?)
  
Am creat o clasa de baza HTTPEndPoint care este Mostenita de doua alte clase pentru a rezolva cele doua cerinte.

1. ClientRespApi : HTTPEndpoint
2. ReviewRestApi : HttpEndPoint

Clasa HTTPEndPoint se afla in biblioteca Utils si contine handlere pentru mesaje ( GET,POST...
Payload-ul este in format JSON

Pentru a pastra unicitatea de 1 player cu 1 singur feedback/ sesiune am pus o cheie unica pe tabela feedback


Am creat si o aplicatie de test TestClient pentru a trimite requesturi catre cele doua EndPointuri
