using UnityEngine;
using UnityEngine.UI;

public class TankHealth : MonoBehaviour
{
    public float m_StartingHealth = 100f;  // Starting health        
    public Slider m_Slider;   // handles values                     
    public Image m_FillImage;    // Fill image to game object                  
    public Color m_FullHealthColor = Color.green;  
    public Color m_ZeroHealthColor = Color.red;    
    public GameObject m_ExplosionPrefab; //
    

    private AudioSource m_ExplosionAudio;  // audio souce for explosion        
    private ParticleSystem m_ExplosionParticles;   // it will spawn game object in the world.
    private float m_CurrentHealth;  
    private bool m_Dead;   // you're dead or not         


    private void Awake()
    {
        m_ExplosionParticles = Instantiate(m_ExplosionPrefab).GetComponent<ParticleSystem>();
        m_ExplosionAudio = m_ExplosionParticles.GetComponent<AudioSource>();

        m_ExplosionParticles.gameObject.SetActive(false); // this useful for game performans
    }


    private void OnEnable()
    {
        m_CurrentHealth = m_StartingHealth;
        m_Dead = false;

        SetHealthUI(); // kind of refresh the game object
    }


    public void TakeDamage(float amount)// depends on distance tank will get damaged. The nearer the more.
    {
        // Adjust the tank's current health, update the UI based on the new health and check whether or not the tank is dead.
        m_CurrentHealth -= amount; // reducing current health 

        SetHealthUI(); // unless we tall the user by updating or refreshing UI
        if(m_CurrentHealth<= 0f && !m_Dead)
        {
            OnDeath();
        }
    }


    private void SetHealthUI()
    {
        // Adjust the value and colour of the slider.
        m_Slider.value = m_CurrentHealth;
        //this fill the slider with color depends on ratio of the health
        m_FillImage.color = Color.Lerp(m_ZeroHealthColor, m_FullHealthColor, m_CurrentHealth / m_StartingHealth);
    }


    private void OnDeath()
    {
        // Play the effects for the death of the tank and deactivate it.
        m_Dead = true;
        //Wherever you were when you died, grab the particles that we put in at the ..
        //..start of the game move them there.
        m_ExplosionParticles.transform.position = transform.position;
        m_ExplosionParticles.gameObject.SetActive(true);

        m_ExplosionParticles.Play(); //Play particles
        m_ExplosionAudio.Play(); // play audio
        gameObject.SetActive(false); // we turn the tank off.


    }
}