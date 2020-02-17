void SendHtmlPage (EthernetClient client, int mode)
{
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">");
  client.println("</head>");
  client.println("<body>");
  client.println("<div id=\"controls\">");
  client.println("<div id=\"header\">");
  client.println("<h1>Living Room Color Kinetics RGB Lights</h1>");
  
  client.println("<ul>");
  Tab (client, 0, "Main", mode <= 1);
  Tab (client, 2, "Fixed Color", mode == 2);
  Tab (client, 3, "Color Wash", mode == 3);
  Tab (client, 4, "Offset Wash", mode == 4);
  Tab (client, 5, "Random", mode == 5);
  Tab (client, 6, "Independent Random", mode == 6);
  Tab (client, 7, "White", mode == 7);
  client.println("</ul>");

  client.println("</div>"); // header
  client.println("<div id=\"content\">");

  switch (mode) {
    case 0: Main (client); break;
    case 1: Main (client); break;
    case 2: FixedColor (client); break;
    case 3: ColorWash (client); break;
    case 4: OffsetWash (client); break;
    case 5: RandomColor (client); break;
    case 6: IndependentRandom (client); break;
    case 7: White (client); break;
}

  client.println ("</div>"); // content
  client.println ("</div>"); // controls
  client.println("</body>");
  client.println("</html>");
}


//---------------------------------------------------------------------------------------------
// TAB
//

void Tab (EthernetClient client, int link, String text, int selected)
{
  if (selected) {
    client.printf ("<li id=\"selected\"><a href=\"/?mode=%d\">%s</a></li>", link, text.c_str());
  } else {
    client.printf ("<li><a href=\"/?mode=%d\">%s</a></li>", link, text.c_str());
  }
}


//---------------------------------------------------------------------------------------------
// MAIN
//

void Main (EthernetClient client)
{
  client.println ("<p style=\"font-weight: bold; font-size: 10pt;\">Available Modes:</p>");
  client.println ("<p>");
  client.println ("<table cellspacing=2 cellpadding=0 class=\"textbuttons\">");
  client.println ("<tr><td>");
  client.println ("<a href=\"/?mode=1\">Off</a>");
  client.println ("</td><td>");
  client.println ("<a href=\"/?mode=2\">Fixed Color</a>");
  client.println ("</td><td>");
  client.println ("<a href=\"/?mode=3\">Color Wash</a>");
  client.println ("</td><td>");
  client.println ("<a href=\"/?mode=4\">Offset Wash</a>");
  client.println ("</td><td>");
  client.println ("<a href=\"/?mode=5\">S Random</a>");
  client.println ("</td><td>");
  client.println ("<a href=\"/?mode=6\">I Random</a>");
  client.println ("</td><td>");
  client.println ("<a href=\"/?mode=7\">White</a>");
  client.println ("</td></tr></table></p>");
}


//---------------------------------------------------------------------------------------------
// FIXED COLOR
//

void FixedColor (EthernetClient client)
{
  client.println ("<p><b>Click a color below:</b></p>");
  client.println ("<p><table cellspacing=2 cellpadding=0 class=\"pallette\">");
  for (int i = 0; i < 1536; i += 16) {
    if ((i % (16*16)) == 0) {
      client.println ("<tr>");
    }

    int r = 0, g = 0, b = 0;
    int hi = i >> 8;
    int lo = i & 0xff;
  
    switch (hi) {
      case 0: 
        r = 0xff;      g = 0x00;      b = lo; 
        break;
      case 1: 
        r = 0xff - lo; g = 0x00;      b = 0xff;
        break;
      case 2: 
        r = 0x00;      g = lo;        b = 0xff;
        break;
      case 3: 
        r = 0x00;      g = 0xff;      b = 0xff - lo;
        break;
      case 4: 
        r = lo;        g = 0xff;      b = 0x00;
        break;
      case 5: 
        r = 0xff;      g = 0xff - lo; b = 0x00;
        break;
    }   
  
    client.print ("<td><a class=\"cell2\" style=\"background:#");
    client.printf ("%02x%02x%02x; display:block;", r, g, b);
    client.print ("\" href=\"/?mode=2&color=");
    client.printf ("%02x%02x%02x", r, g, b);
    client.println ("\"></a></td>");

    if ((i % (16*16)) == 15) {
      client.println ("</tr>");
    }
  }
  client.println ("</table></p>");
}


//---------------------------------------------------------------------------------------------
// COLOR WASH
//

void ColorWash (EthernetClient client)
{
  client.println ("<p style=\"font-weight: bold; font-size: 10pt;\">Direction:</p>");
  client.println ("<p>");
  client.println ("<table cellspacing=2 cellpadding=0 class=\"textbuttons\">");
  client.println ("<tr><td>");
  client.println ("<a href=\"/?mode=3&direction=0\">Forward</a>");
  client.println ("</td><td>");
  client.println ("<a href=\"/?mode=3&direction=1\">Reverse</a>");
  client.println ("</td></tr></table></p>");

  client.println ("<p style=\"font-weight: bold; font-size: 10pt;\">Cycle Time:</p>");
  client.println ("<p>");
  client.println ("<table cellspacing=2 cellpadding=0 class=\"textbuttons\">");
  client.println ("<tr>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=1EB8\">1 second</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=0F5C\">2 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=0A3D\">3 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=0624\">5 seconds </a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=0312\">10 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=020C\">15 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=0106\">30 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=00AF\">45 seconds</a></td>");
  client.println ("</tr><tr>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=0083\">1 minute</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=0041\">2 minutes</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=002C\">3 minutes</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=001A\">5 minutes</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=000D\">10 minutes</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=0009\">15 minutes</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=0007\">20 minutes</a></td>");
  client.println ("<td><a href=\"/?mode=3&cycle_time=0004\">30 minutes</a></td>");
  client.println ("</tr></table></p>");
}


//---------------------------------------------------------------------------------------------
// OFFSET WASH
//

void OffsetWash (EthernetClient client)
{
  client.println ("<p style=\"font-weight: bold; font-size: 10pt;\">Direction:</p>");
  client.println ("<p>");
  client.println ("<table cellspacing=2 cellpadding=0 class=\"textbuttonsshort\">");
  client.println ("<tr><td>");
  client.println ("<a href=\"/?mode=4&direction=0\">Forward</a>");
  client.println ("</td><td>");
  client.println ("<a href=\"/?mode=4&direction=1\">Reverse</a>");
  client.println ("</td></tr></table></p>");

  client.println ("<p style=\"font-weight: bold; font-size: 10pt;\">Cycle Time:</p>");
  client.println ("<p>");
  client.println ("<table cellspacing=2 cellpadding=0 class=\"textbuttonsshort\">");
  client.println ("<tr>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=1EB8\">1 second</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=0F5C\">2 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=0A3D\">3 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=0624\">5 seconds </a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=0312\">10 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=020C\">15 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=0106\">30 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=00AF\">45 seconds</a></td>");
  client.println ("</tr><tr>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=0083\">1 minute</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=0041\">2 minutes</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=002C\">3 minutes</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=001A\">5 minutes</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=000D\">10 minutes</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=0009\">15 minutes</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=0007\">20 minutes</a></td>");
  client.println ("<td><a href=\"/?mode=4&cycle_time=0004\">30 minutes</a></td>");
  client.println ("</tr></table></p>");

  client.println ("<p style=\"font-weight: bold; font-size: 10pt;\">Offset:</p>");
  client.println ("<p>");
  client.println ("<table cellspacing=2 cellpadding=0 class=\"textbuttonsshort\">");
  client.println ("<tr>");
  client.println ("<td><a href=\"/?mode=4&wash_offset=0180\">1/4 Cycle</a></td>");
  client.println ("<td><a href=\"/?mode=4&wash_offset=00C0\">1/8 Cycle</a></td>");
  client.println ("<td><a href=\"/?mode=4&wash_offset=0080\">1/12</a></td>");
  client.println ("<td><a href=\"/?mode=4&wash_offset=0060\">1/16</a></td>");
  client.println ("<td><a href=\"/?mode=4&wash_offset=0040\">1/24</a></td>");
  client.println ("<td><a href=\"/?mode=4&wash_offset=0030\">1/32</a></td>");
  client.println ("<td><a href=\"/?mode=4&wash_offset=0020\">1/48</a></td>");
  client.println ("<td><a href=\"/?mode=4&wash_offset=0018\">1/64</a></td>");
  client.println ("</tr>");
  client.println ("<tr>");
  client.println ("<td><a href=\"/?mode=4&wash_offset=0013\">1/80</a></td>");
  client.println ("<td><a href=\"/?mode=4&wash_offset=0010\">1/96</a></td>");
  client.println ("<td><a href=\"/?mode=4&wash_offset=0008\">1/192</a></td>");
  client.println ("</tr>");
  client.println ("</table>");
  client.println ("</p>");
}


//---------------------------------------------------------------------------------------------
// RANDOM COLOR
//

void RandomColor (EthernetClient client)
{
  client.println ("<p style=\"font-weight: bold; font-size: 10pt;\">Hold Time:</p>");
  client.println ("<p><table cellspacing=2 cellpadding=0 class=\"textbuttons\"><tr>");
  client.println ("<td><a href=\"/?mode=5&random_delay=3000\">1 minute</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=2250\">45 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=1500\">30 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=1250\">25 seconds </a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=1000\">20 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=750\">15 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=500\">10 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=250\">5 seconds</a></td>");
  client.println ("</tr><tr>");
  client.println ("<td><a href=\"/?mode=5&random_delay=150\">3 second</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=100\">2 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=75\">1.5 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=50\">1 second</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=38\">0.75 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=25\">0.5 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=12\">0.25 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=5&random_delay=5\">0.10 seconds</a></td>");
  client.println ("</tr></table></p>");
}


//---------------------------------------------------------------------------------------------
// INDEPENDENT RANDOM
//

void IndependentRandom (EthernetClient client)
{
  client.println ("<p style=\"font-weight: bold; font-size: 10pt;\">Hold Time:</p>");
  client.println ("<p><table cellspacing=2 cellpadding=0 class=\"textbuttons\"><tr>");
  client.println ("<td><a href=\"/?mode=6&random_delay=3000\">1 minute</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=2250\">45 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=1500\">30 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=1250\">25 seconds </a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=1000\">20 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=750\">15 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=500\">10 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=250\">5 seconds</a></td>");
  client.println ("</tr><tr>");
  client.println ("<td><a href=\"/?mode=6&random_delay=150\">3 second</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=100\">2 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=75\">1.5 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=50\">1 second</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=38\">0.75 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=25\">0.5 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=12\">0.25 seconds</a></td>");
  client.println ("<td><a href=\"/?mode=6&random_delay=5\">0.10 seconds</a></td>");
  client.println ("</tr></table></p>");
}


//---------------------------------------------------------------------------------------------
// WHITE
//

void White (EthernetClient client)
{
  client.println ("<p><b>Click a level below:</b></p>");
  client.println ("<p><table cellspacing=2 cellpadding=0 class=\"pallette\"><tr>");
  for (int i = 0; i < 256; i += 17) {

    client.print ("<td><a class=\"cell3\" style=\"background:#");
    client.printf ("%02x%02x%02x; display:block;", i, i, i);
    client.print ("\" href=\"/?mode=7&level=");
    client.printf ("%d", i);
    client.println ("\"></a></td>");

    if (i == 0x77) {
      client.println ("</tr><tr>");
    }
  }
  client.println ("</tr></table></p>");
}
