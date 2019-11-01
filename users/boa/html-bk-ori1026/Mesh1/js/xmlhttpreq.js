/*
 *创建异步访问对象
 */
function createXHR() 
{
    var xhr;


    try 
    {
        xhr = new ActiveXObject("Msxml2.XMLHTTP");
    } 
    catch (e) 
    {
        try 
        {
            xhr = new ActiveXObject("Microsoft.XMLHTTP");
        }
        catch(E) 
        {
            xhr = false;
        }
    }

    if (!xhr && typeof XMLHttpRequest != 'undefined') 
    {
        xhr = new XMLHttpRequest();
    }

    return xhr;
}

/*
 *异步访问提交处理
 */
function sender() 
{
    xhr = createXHR();

    if(xhr)
    {
        xhr.onreadystatechange=callbackFunction;
    
        //test.cgi后面跟个cur_time参数是为了防止Ajax页面缓存cgi的目录一定不能错，否则页面无法正常运行
        xhr.open("GET", "../c_demo.cgi?cur_time=" + new Date().getTime());
        xhr.send(null);
    }
    else
    {
        //XMLHttpRequest对象创建失败
        alert("浏览器不支持，请更换浏览器！");
    }
}

/*
 *异步回调函数处理
 */
function callbackFunction()
{
    if (xhr.readyState == 4) 
    {
        if (xhr.status == 200) 
        {
			/* content-type:text/html ret:upspeed=%dTX&downspeed=%dRX */
			var speedUp, speedDown;
            var returnValue = xhr.responseText;

            if(returnValue != null && returnValue.length > 0)
            {
				var retLen = returnValue.length;
				var pos00 = returnValue.search("ret:upspeed=");
				var pos01 = returnValue.search("TX");
				var pos10 = returnValue.search("&downspeed=");
				var pos11 = returnValue.search("RX");
				
				if (pos00 == -1|| pos01 == -1)
					speedUp = 0;
				else
					speedUp = returnValue.slice("ret:upspeed=".length+pos00, pos01);
				
				if (pos10 == -1|| pos11 == -1)
					speedDown = 0;
				else
					speedDown = returnValue.slice("&downspeed=".length+pos10, pos11);
				
                document.getElementById("wanUplinkRate").innerHTML = speedUp ;
				document.getElementById("wanDownlinkRate").innerHTML = speedDown ;
            }
            else
            {
                alert("结果为空！");
            }
        } 
        else 
        {
            alert("页面出现异常！");
        }
    }
}
