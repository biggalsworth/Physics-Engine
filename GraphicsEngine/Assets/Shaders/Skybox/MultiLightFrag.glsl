#version 330 core
layout(location = 0) out vec4 fragmentColour;

in vec3 LightingColour;
in vec2 textCord;
uniform float offset;

//uniform vec3 objColour;
uniform vec3 lightCol;


struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //sampler2D diffuse;
    //sampler2D specular;
    float shininess;
}; 


struct PointLight{    
    vec3 position;
    vec3 lightColour;

    float constant;
    float linear;
    float quadratic; 
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct directLight{
    vec3 lightColour;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

struct spotLight {
    vec3 position;
    vec3 direction;
    //the phi
    float cutOff;
    // the fade out of the light angle
    float outCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 
};

const int lightAmount = 4;
const int spotAmount = 1;
uniform PointLight pointLights[lightAmount];
uniform spotLight spotLights[spotAmount];
uniform directLight directional;

uniform Material material;

in vec3 vertColour;
in vec3 TotalColour;
in vec3 fragNorm;
in vec3 FragPos;

uniform vec3 viewPos;

uniform sampler2D colourTexture;
uniform sampler2D normalMap;
out vec4 finalColour;

//uniform vec3 objectColour = vec3(0.89, 0.00, 1.00);

vec3 CalcPoint(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcLight(directLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpot(spotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    //normal mapping
    vec3 normalColour = fragNorm * 0.5 + 0.5; // transforms the rgb of the image, [-1,1] to [0,1]  
    normalColour = texture(normalMap, textCord).rgb;
    vec3 norm = normalize(normalColour * 2.0 - 1.0);


    vec3 viewDir = normalize(viewPos - FragPos);


    vec3 result;
    for(int i = 0; i < lightAmount; i++)
    {
        //result += CalcPoint(pointLights[i], norm, FragPos, viewDir);
    }
    for(int i = 0; i < spotAmount; i++)
    {
        //result += CalcSpot(spotLights[0], norm, FragPos, viewDir);
    }


    result += CalcLight(directional, norm, FragPos, viewDir);


    vec2 newTextCord = vec2(textCord.x + offset, textCord.y);
    finalColour = texture(colourTexture, newTextCord);
    fragmentColour = finalColour * vec4(result, 1.0f);

}


vec3 CalcPoint(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    
    float ambientStrength = 0.1;
    float specularStrength = 0.5;
    float specRadius = 64;
    vec3 ambient = ambientStrength * light.lightColour * material.ambient;

    //diffuse
    //normal = normalize(fragNorm);
    //direction of vector going from light to the fragment
    vec3 lightDirection = normalize(light.position - FragPos);
    float diff = max(dot(fragNorm, lightDirection), 0.0);
    vec3 diffuse = diff * light.lightColour * material.diffuse;

    //Specular
    //vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return(ambient + diffuse + specular);
}


vec3 CalcLight(directLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // how will it reflect to get the specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);

}

vec3 CalcSpot(spotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //get direction of spotlight to the frag
    vec3 lightDir = normalize(light.position - fragPos);

    // get the diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // how will it reflect to get the specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // spotlight specific attributes
    //where the spotlight has hit the fragment
    float theta = dot(lightDir, normalize(-light.direction)); 
    //the space within the spotlight cone
    float epsilon = light.cutOff - light.outCutOff;
    float intensity = clamp((theta - light.outCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);

}



