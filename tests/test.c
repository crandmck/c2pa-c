// Copyright 2023 Adobe. All rights reserved.
// This file is licensed to you under the Apache License,
// Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
// or the MIT license (http://opensource.org/licenses/MIT),
// at your option.
// Unless required by applicable law or agreed to in writing,
// this software is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR REPRESENTATIONS OF ANY KIND, either express or
// implied. See the LICENSE-MIT and LICENSE-APACHE files for the
// specific language governing permissions and limitations under
// each license.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/c2pa.h"
#include "unit_test.h"

// const char *manifest = "{\
//     \"claim_generator\":\"test\",\
//     \"ingredients\":[], \
//     \"assertions\":[] \
// }";


int main(void)
{
    char *version = c2pa_version();
    assert_contains("version", version, "c2pa-c/0.");

    char *formats = c2pa_supported_formats();
    assert_contains("c2pa_supported_formats", formats, "jpeg");

   char *result1 = c2pa_read_file("tests/fixtures/C.jpg", NULL);
    assert_not_null("c2pa_read_file_no_data_dir", result1);

    char *result = c2pa_read_file("tests/fixtures/C.jpg", "target/tmp");
    assert_not_null("c2pa_read_file", result);

    result = c2pa_ingredient_from_file("tests/fixtures/C.jpg", "target/ingredient");
    assert_not_null("c2pa_ingredient_from_file", result);
  
    char *certs = load_file("tests/fixtures/es256_certs.pem");
    char *private_key = load_file("tests/fixtures/es256_private.key");

    char *manifest = load_file("tests/fixtures/training.json");
    
    // create a sign_info struct
    C2paSignerInfo sign_info = {.alg = "es256", .tsa_url = "http://timestamp.digicert.com", .signcert = certs, .pkey = private_key};
   
    result = c2pa_sign_file("tests/fixtures/C.jpg", "target/tmp/earth.jpg", manifest, sign_info, "tests/fixtures");
    assert_not_null("c2pa_sign_file_ok", result);

    result = c2pa_sign_file("tests/fixtures/foo.jpg", "target/tmp/earth.jpg", manifest, sign_info, "tests/fixtures");
    assert_null("c2pa_sign_file_not_found", result, "FileNotFound");

    result = c2pa_sign_file("tests/fixtures/es256_certs.pem", "target/tmp/earth.jpg", manifest, sign_info, "tests/fixtures");
    assert_null("c2pa_sign_file_not_supported", result, "NotSupported");

    free(certs);
    free(private_key);

}