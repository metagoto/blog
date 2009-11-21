
//global pathinfo was set before

routes = {
  
  index: {
    pat: /^(?:index)?(?:page\/(\d{1,4}))?$/,
    map: { 1: "page" }
  },
  post: {
    pat: /^post\/([^\/]+)$/,
    map: {1: "id"},
    POST: "post_post"
  },
  category: {
    pat: /^category\/([^\/]+)(?:\/page\/(\d{1,4}))?$/,
    map: {1: "id", 2: "page"}
  },
  tag: {
    pat: /^tag\/([^\/]+)(?:\/page\/(\d{1,4}))?$/,
    map: {1: "id", 2: "page"}
  },
  preview: {
    pat: /^preview$/,
    GET: "blank"
  },
  about: {
    pat: /^about$/
  },
  help: {
    pat: /^help$/
  },

  admin_index: {
    pat: /^adm$/
  },
  admin_edit: {
    pat: /^adm\/edit\/([^\/]+)$/,
    map: {1: "id"},
    POST: "admin_edit_post"
  },
  admin_new: {
    pat: /^adm\/new$/,
    POST: "admin_new_post"
  },
  admin_preview: {
    pat: /^adm\/preview$/,
    GET: "blank"
  },

  login: {
    pat: /^login$/,
    POST: "try_login"
  },

  reset: { // tmp reset
    pat: /^reset$/
  },
  
  check: { // tmp check cache
    pat: /^check$/
  },

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


