import xmlrpc.client

url = "http://<ip>/pub/xmlrpc/client/"
user = "dichen"
passwd = "tianwanggaidihu"

class CookiesTransport(xmlrpc.client.Transport):
    """Retains cookies over its lifetime."""

    # Note context option - it's required for success
    def __init__(self):
        super().__init__()
        self._cookies = []

    def send_headers(self, connection, headers):
        if self._cookies:
            connection.putheader("Cookie", "; ".join(self._cookies))
        super().send_headers(connection, headers)

    def parse_response(self, response):
        # This check is required if in some responses we receive no cookies at all
        if response.msg.get_all("Set-Cookie"):
            for header in response.msg.get_all("Set-Cookie"):
                cookie = header.split(";", 1)[0]
                self._cookies.append(cookie)
        return super().parse_response(response)

transport = CookiesTransport()
server = xmlrpc.client.ServerProxy(url, transport=transport)
sessionid = server.auth.login_password(user, passwd)
info = server.admin.list_push_permissions(user)
print(info)
