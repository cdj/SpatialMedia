LineJumper
==========

A game that decides how long you have to wait in line

There are two UI entry points:

1) dashboard.php - this would be loaded on a single machine, displaying the wait times to all the participants

2) client.php - this would be loaded on the machines of participants, probably smart phones
              - url would have a query string specifying the participant's team, like: ?teamNum=2
              - url would probably be loaded from a QR code
