		
		
		function do_count_down(count)
		{
			if(count == 0) 
			{
				var old_url=window.location.href;
				var https;
                if(old_url.indexOf("https:")>=0)
                {
                    https=1;
                }
                else
                {
                    https=0;
                }
                var lastUrl="<% getInfo("lastUrl"); %>";
//              var lastUrl="ddddd";
				
				if(lastUrl == "/wizard.htm")
				{
                    if(https==1)
                        parent.location.href = 'https://'+connect_url;
                    else
                        parent.location.href = 'http://'+connect_url;
                }
				else
				{
				    if(https==1)	
                    {
                        var location_href = 'https://'+connect_url+'?t='+new Date().getTime();				
                        parent.location.href = 'https://'+connect_url;
                    }else
                    {
					var location_href = 'http://'+connect_url+'?t='+new Date().getTime();								
					parent.location.href = 'http://'+connect_url;
                    }
				}
			}
		}