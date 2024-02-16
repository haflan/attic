package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"encoding/base64"
	"strings"

	//"encoding/json"
	"fmt"
	"os"
)

var secretKey []byte
var encodedJWTHeader string = base64.RawURLEncoding.EncodeToString([]byte(`{"alg":"HS256","typ":"JWT"}`))

func ComputeHmac256(message string) string {
	h := hmac.New(sha256.New, secretKey)
	h.Write([]byte(message))
	return base64.RawURLEncoding.EncodeToString(h.Sum(nil))
}

func ComputeJWT(payload string) string {
	data := encodedJWTHeader + "." + base64.RawURLEncoding.EncodeToString([]byte(payload))
	return fmt.Sprintf("%v.%v", data, ComputeHmac256(data))
}

func JWTValid(jwt string) bool {
	parts := strings.Split(jwt, ".")
	if len(parts) != 3 {
		return false
	}
	return parts[2] == ComputeHmac256(parts[0]+"."+parts[1])
}

const useMessage = `Commands:
verify <jwt> : Verify JWT
gen <payload> : Generate JWP from arbitrary payload
`

func main() {
	key := os.Getenv("JWTEX_KEY")
	if key == "" {
		fmt.Println("Please define env var JWTEX_KEY")
		os.Exit(1)
	}
	secretKey = []byte(key)
	if len(os.Args) < 3 {
		fmt.Println(useMessage)
		os.Exit(0)
	}
	if os.Args[1] == "gen" {
		fmt.Println(ComputeJWT(os.Args[2]))
	} else {
		if JWTValid(os.Args[2]) {
			fmt.Println("JWT valid")
		} else {
			fmt.Println("JWT invalid!")
		}
	}
}
