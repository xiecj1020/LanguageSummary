package com.crab.test.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class MyIndex {
    @RequestMapping("index")
    public @ResponseBody String loadString(){
        return "This is my first spring demo.";
    }
}
