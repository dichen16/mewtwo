extern crate xmlrpc;
extern crate reqwest;

use xmlrpc::{Request, Transport};
use xmlrpc::http::{build_headers, check_response};

use reqwest::{Client, RequestBuilder};
use reqwest::header::COOKIE;

use std::error::Error;

struct MyTransport(RequestBuilder);

impl Transport for MyTransport {
    type Stream = reqwest::Response;
    fn transmit(self, request: &Request) -> Result<Self::Stream, Box<dyn Error + Send + Sync>> {
        let mut body = Vec::new();
        request.write_as_xml(&mut body).expect("could not write request to buffer (this should never happen)");
        let response = build_headers(self.0, body.len() as u64)
            .header(COOKIE, "SESSION=123abc")  // Our custom header will be a `Cookie` header
            .body(body)
            .send()?;
        check_response(&response)?;
        Ok(response)
    }
}

//used to request pub targets
fn main() {
    let request = Request::new("client.list_targets");
    let tp = MyTransport(Client::new().post("http://10.0.151.168/pub/xmlrpc/client"));
    let result = request.call(tp);
    println!("Result: {:?}", result);
}
