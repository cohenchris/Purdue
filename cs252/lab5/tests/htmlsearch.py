import sys
from html.parser import HTMLParser
import urllib.parse

class MyHTMLParser(HTMLParser):
    def __init__(self):
      super().__init__()
    def handle_starttag(self, tag, attrs):
        # print(tag, attrs)
        if ("a" == tag and len(attrs[0]) == 2 and "href" == attrs[0][0]):
            print(urllib.parse.unquote(attrs[0][1]))

parser = MyHTMLParser()
with open(sys.argv[1], "r") as f:
    contents = f.read()
    f.close()
    parser.feed(contents)
    exit(1)

