#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;   //--- 조명의 위치
uniform vec3 lightColor; //--- 조명의 색
uniform vec3 objectColor; //--- 객체의 색

void main()
{
    vec3 ambientLight = vec3(0.3);
    vec3 ambient = ambientLight * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diffuseLight = max(dot(norm, lightDir), 0.0);
    float diffuse = diffuseLight * lightColor.r;

    float shininess = 32.0; // 수정: 광택 정도를 나타내는 shininess 값 변경

    vec3 viewDir = normalize(-FragPos); // 수정: 간단한 방향광이므로 뷰 방향은 카메라에서 조명까지의 반대 방향

    vec3 reflectDir = reflect(-lightDir, norm);
    float specularLight = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularLight * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
