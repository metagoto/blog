
//global pathinfo was set before

routes = {
  
  category: {
    pat: /^category\/([^\/]+)(?:\/page\/(\d{1,4}))?$/,
    map: {1: "id", 2: "page"}
  },
  
  tag: {
    pat: /^tag\/([^\/]+)(?:\/page\/(\d{1,4}))?$/,
    map: {1: "id", 2: "page"}
  },

  post: {
    pat: /^post\/([^\/]+)$/,
    map: {1: "id"},
    POST: "post_post"
  },

  preview: {
    pat: /^preview$/
  },

  about: {
    pat: /^about$/
  },

  help: {
    pat: /^help$/
  },

  reset: { // reset
    pat: /^reset$/
  },
  
  check: { // check cache
    pat: /^check$/
  },

  index: {
    pat: /^(?:index)?(?:page\/(\d{1,4}))?$/,
    map: { 1: "page" }
  }

}

function route(routes) {
  
  var path = PATH_INFO.replace(/^\/|\/$/g, "");

  var res = { route: "error", params: [] };
  
  for (var i in routes) {
    var match, route = routes[i];
    if (match = path.match(route.pat)) {
     
      res.route = route[REQUEST_METHOD] || i;
      
      if (route.map) {
        for (var k in route.map)
          res.params.push({name: route.map[k], value: match[k]});
      }
      break;
    }
  }
  return setRoute(res);
};

//log(params.REQUEST_METHOD);

route(routes);


