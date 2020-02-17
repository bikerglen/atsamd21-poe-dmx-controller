void SendStyleSheet (EthernetClient client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/css");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println();
  client.println ("body { font: 9pt arial, helvetica, sans-serif; background-color: #c0c0c0; } ");
  client.println ("#controls { width: 784px; } ");
  client.println ("#header h1 { margin: 0; padding: 0px 0 5px 0; font-size: 12pt; } ");
  client.println ("#header ul { list-style: none; padding: 0; margin: 0; } ");
  client.println ("#header li { float: left; border: 1px solid #bbb; border-bottom-width: 0; margin: 0; } ");
  client.println ("#header a { text-decoration: none; display: block; background: #eee; padding: 4px 16px; color: #00c; min-width:4em; text-align: center; } ");
  client.println ("#header a:hover { background: #ddf; } ");
  client.println ("#header #selected { border-color: black; } ");
  client.println ("#header #selected a { background: white; color: black; font-weight: bold; padding-bottom: 6px; margin-bottom: -2px; } ");
  client.println ("#content { border-top: 1px solid black; border-left: 1px solid black; border-right: 1px solid black; border-bottom: 1px solid black; clear: both; padding: 0 1em 1em 1em; background: white; min-height: 400px; } ");
  client.println ("#content table.pallette td { border: 0px 0px 0px 0px; margin: 5px 5px 5px 5px; padding: 0 0 0 0px; } ");
  client.println ("#content table.textbuttons a { display: block; width: 88px; border: 1px solid black; padding-top: 32px; padding-bottom: 32px; text-align: center; color: black; text-decoration: none; font-weight: bold; font-size: 10pt; } ");
  client.println ("#content table.textbuttonsshort a { display: block; width: 88px; border: 1px solid black; padding-top: 18px; padding-bottom: 18px; text-align: center; color: black; text-decoration: none; font-weight: bold; font-size: 10pt; } ");
  client.println ("#content h1 { margin: 10px 0px 0px 0px; padding: 0px 0px 0px 0px; font-size: 24px; text-align: center; } ");
  client.println ("#content p { margin: 10px 0px 0px 0px; padding: 0px 0px 0px 0px; } ");
  client.println ("#content select.form2 { font-size: 8pt; width: 150px; } ");
  client.println ("#content form { margin: 0; padding: 0; font-size: 8pt; } ");
  client.println ("#content select.form { font-size: 8pt; } ");
  client.println ("#content input.form { font-size: 8pt; } ");
  client.println ("#footer { border-bottom: 1px solid black; border-left: 1px solid black; border-right: 1px solid black; clear: both; padding: 1em 1em 0 1em; background: white; } ");
  client.println ("#footer p { font-size: 8pt; text-align: right; text-valign: bottom; margin-top: 0px; margin-bottom: 1em; } ");
  client.println ("#footer a { color: #00c; } ");
  client.println ("#content a.cell2 { width:43px; height:43px; border-left:1px solid #000; border-right:1px solid #000; border-top:1px solid #000; border-bottom:1px solid #000; margin: 0px 0px 0px 0px; }");
  client.println ("#content a.cell3 { width:90px; height:90px; border-left:1px solid #000; border-right:1px solid #000; border-top:1px solid #000; border-bottom:1px solid #000; margin: 0px 0px 0px 0px; }");
}
