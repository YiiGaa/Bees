# _ServerHttp-Http/Https Request

The main functions include:

- Http/Https Request

Description of the value of module parameters:

- If the value you want to set is a fixed value, you can directly set the value of the corresponding type.

- If you want to dynamically obtain the value from `passParam` as the set value, please use `get##key` as the setting value of the module parameter.

- If the value you want to get is in the inner layer of passParam (nested json), please locate it with `>>`, such as `get##key_1>>key_2`

# ※ Send Http/Https Request

- Add version: 1.0=2025.01.12
- The status code returned by the request needs to be 2XX, otherwise the request will be considered abnormal and an error will be reported

```
{
	"name":"_ServerHttp",
	"param":{
    "_url":"",
    "_method":"",
    "_param":"",
    "_resultKey":""
	}
}
```

| Key       | Description                     | Necessary | Default                | Type | Add/Update  |
| --------- | ------------------------ | -------- | --------------------- | ------ | -------------- |
| _url | The requested URL | Yes |         | String | 1.0=2025.01.12 |
| _method | Request method, POST/DELETE/PUT/PATCH/GET | No | POST | String | 1.0=2025.01.12 |
| _param | Request body | No |         | Object | 1.0=2025.01.12 |
| _paramType | Request Body Type (Content-Type) | No | application/json | String | 1.0=2024.03.16 |
| _header | Request header settings | No |         | Object | 1.0=2024.04.27 |
| _resultKey | The data returned by the request is stored in the key of passParam | Yes |  | String | 1.0=2024.04.27 |
| _isVerifySSL | When the request is https, whether to verify the ssl certificate, false: no authentication, true: authentication | No | false | Bool | 1.0=2024.04.27 |

Supplementary description of module parameters:

### > _url

- The requested URL

- Please set the full URL, such as https://stoprefactoring.com/test/test

- When the request method is GET, it is recommended to set the parameter in `_param`

### > _method

- Request methods, supporting 5 types of request methods, POST/DELETE/PUT/PATCH/GET

- It needs to be filled in strictly according to the capitalized words, otherwise it will report an error

### > _param

- Request body

- It is recommended to use the Json object `{}` 

- In fact, this parameter is an open type that can pass strings, numbers, bool, arrays `[]`, etc.

- When the request method is `GET` (some services may reject the request body of GET) and the current parameter is Json object `{}`, it will be automatically spliced into _url, such as `?key1=value1&key2=value2`

- When the request body is automatically spliced to _url, key and value will be automatically URL encoded

### > _isVerifySSL

- When the request is https, whether to verify the ssl certificate, false: no authentication, true: authentication

- The verification is the SSL certificate of the server. If it is tested or the self-visa certificate is used, it needs to be set to `false`, otherwise an error will be reported

# ◎ Configuration instructions

The module configuration is set in `_ServerHttp/Once.Config`. When running the application through Christmas, it will be automatically spliced to `/Kernel/Common/Config/Config.hpp`

| key | Description | Default value | Type | Group/Update version |
| --------------------------------- | ----------------- | ------ | ---- | -------- |
| CONFIG__ServerHttp_TimeoutConnect | Connection Timeout Time (Seconds) | 10 | Int | 1.0=2025.01.12 |
| CONFIG__ServerHttp_TimeoutRead | Read Timeout Time (Seconds) | 5 | Int | 1.0=2025.01.12 |
| CONFIG__ServerHttp_TimeoutWrite | Write timeout time (seconds) | 5 | Int | 1.0=2025.01.12 |

# ◎ Module test separately

In the project root directory, run the shell instruction

```
python3 Christmas/Christmas.py ShellExcute/Build#Module _ServerHttp
```

You can also run `ShellExcute>>Build#Module` through the `Christmas` plug-in, and enter `_ServerHttp` in the terminal opened by the plug-in

`Sample.html`is a code specifically for separate testing

The module will run automatically after compilation, and the compiled executable file will be generated in `/Build/sample`

# ◎ Update list

**1.0=2025.01.12**

- Module create