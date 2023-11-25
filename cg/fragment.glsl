#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;   //--- ������ ��ġ
uniform vec3 lightColor; //--- ������ ��
uniform vec3 objectColor; //--- ��ü�� ��

void main()
{
    vec3 ambientLight = vec3(0.3);
    vec3 ambient = ambientLight * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diffuseLight = max(dot(norm, lightDir), 0.0);
    float diffuse = diffuseLight * lightColor.r;

    float shininess = 32.0; // ����: ���� ������ ��Ÿ���� shininess �� ����

    vec3 viewDir = normalize(-FragPos); // ����: ������ ���Ɽ�̹Ƿ� �� ������ ī�޶󿡼� ��������� �ݴ� ����

    vec3 reflectDir = reflect(-lightDir, norm);
    float specularLight = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularLight * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
